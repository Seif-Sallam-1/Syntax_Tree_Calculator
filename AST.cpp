#include "AST.h"  // Correctly including the .h file
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

using namespace std;

// --- BNode Implementation ---
AST::BNode::BNode(string val, BNode *l, BNode *r)
    : data(val), left(l), right(r) {
}

AST::BNode::~BNode() {
    delete left;
    delete right;
}

// --- AST Private Helpers ---
int AST::getPrecedence(const string &op) {
    if (op == "_NEG_") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

bool AST::isOperator(const string &part) {
    return part == "+" || part == "-" || part == "*" || part == "/" || part == "_NEG_";
}


double AST::calculateRecursive(BNode *node) {
    if (!node) return 0.0;
    if (!isOperator(node->data)) {
        try { return stod(node->data); } catch (const exception &e) {
            throw runtime_error("Error: Non-numerical operand '" + node->data + "' found.");
        }
    }
    if (node->data == "_NEG_") return -calculateRecursive(node->right);

    double leftValue = calculateRecursive(node->left);
    double rightValue = calculateRecursive(node->right);

    if (node->data == "+") return leftValue + rightValue;
    if (node->data == "-") return leftValue - rightValue;
    if (node->data == "*") return leftValue * rightValue;
    if (node->data == "/") {
        if (rightValue == 0.0) throw runtime_error("Error: Division by zero.");
        return leftValue / rightValue;
    }
    throw runtime_error("Error: Unknown operator: " + node->data);
}

AST::~AST() { delete head; }

AST::AST(string expression) : head(nullptr) {
    vector<string> parts_raw;
    string current_part;
    for (int i = 0; i < expression.length(); ++i) {
        char c = expression[i];
        if (c == ' ') {
            if (!current_part.empty()) {
                parts_raw.push_back(current_part);
                current_part.clear();
            }
            continue;
        }
        string op_str(1, c);
        if (op_str == "+" || op_str == "-" || op_str == "*" || op_str == "/" || op_str == "(" || op_str == ")") {
            if (!current_part.empty()) {
                parts_raw.push_back(current_part);
                current_part.clear();
            }
            parts_raw.push_back(op_str);
        } else {
            current_part += c;
        }
    }
    if (!current_part.empty()) parts_raw.push_back(current_part);

    if (!parts_raw.empty()) {
        if (parts_raw[0] == "-") parts_raw[0] = "_NEG_";
        for (int i = 1; i < parts_raw.size(); ++i) {
            if (parts_raw[i] == "-") {
                const string &prev_part = parts_raw[i - 1];
                if (isOperator(prev_part) || prev_part == "(") parts_raw[i] = "_NEG_";
            }
        }
    }

    stack<string> opStack;
    for (const string &part: parts_raw) {
        if (!isOperator(part) && part != "(" && part != ")") postfixContainer.push_back(part);
        else if (part == "(") opStack.push(part);
        else if (part == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                postfixContainer.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        } else if (isOperator(part)) {
            while (!opStack.empty() && opStack.top() != "(" && getPrecedence(opStack.top()) >= getPrecedence(part)) {
                postfixContainer.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(part);
        }
    }
    while (!opStack.empty()) {
        if (opStack.top() == "(") throw runtime_error("Error: Unmatched left parenthesis.");
        postfixContainer.push_back(opStack.top());
        opStack.pop();
    }
}

void AST::buildTree() {
    if (postfixContainer.empty()) {
        cout << "Warning: Empty postfix." << endl;
        return;
    }
    stack<BNode *> nodeStack;
    for (const string &part: postfixContainer) {
        if (part == "_NEG_") {
            if (nodeStack.size() < 1) throw runtime_error("Missing operand for unary -");
            BNode *right = nodeStack.top();
            nodeStack.pop();
            nodeStack.push(new BNode(part, nullptr, right));
        } else if (isOperator(part)) {
            if (nodeStack.size() < 2) throw runtime_error("Missing operands for " + part);
            BNode *right = nodeStack.top();
            nodeStack.pop();
            BNode *left = nodeStack.top();
            nodeStack.pop();
            nodeStack.push(new BNode(part, left, right));
        } else {
            nodeStack.push(new BNode(part));
        }
    }
    if (nodeStack.size() != 1) throw runtime_error("Error: Malformed expression.");
    head = nodeStack.top();
}

double AST::calculate() {
    if (!head) throw runtime_error("Tree not built.");
    return calculateRecursive(head);
}

AST::BNode *AST::getRoot() { return head; }
