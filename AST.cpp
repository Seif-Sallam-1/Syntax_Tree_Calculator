#include "AST.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <cmath>
#include <algorithm>
#include <format>

using namespace std;


void AST::StringArray::add(string s) {
    string* newData = new string[size + 1];

    for (int i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    newData[size] = s;

    delete[] data;
    data = newData;
    size++;
}

void AST::StringArray::clear() {
    delete[] data;
    data = nullptr;
    size = 0;
}


AST::BNode::BNode(string val, BNode *l, BNode *r)
    : data(val), left(l), right(r) {}

AST::BNode::~BNode() {
    delete left;
    delete right;
}

AST::AST(const AST& other) : head(nullptr) {
    if (other.head) head = copyTree(other.head);
}

AST& AST::operator=(const AST& other) {
    if (this != &other) {
        delete head;
        head = other.head ? copyTree(other.head) : nullptr;
    }
    return *this;
}

AST::BNode* AST::copyTree(BNode* node) {
    if (!node) return nullptr;
    return new BNode(node->data, copyTree(node->left), copyTree(node->right));
}

int AST::getPrecedence(const string &op) {
    if (op == "_NEG_") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

bool AST::isOperator(const string &part) {
    return part == "+" || part == "-" || part == "*" || part == "/" || part == "_NEG_";
}

AST::StringArray AST::tokenize(const string& expression) {
    StringArray parts;
    string current_part;
    for (char c : expression) {
        string op_str(1, c);
        if (op_str == "+" || op_str == "-" || op_str == "*" || op_str == "/" || op_str == "(" || op_str == ")") {
            if (!current_part.empty()) {
                parts.add(current_part);
                current_part.clear();
            }
            parts.add(op_str);
        } else {
             current_part += c;
        }
    }
    if (!current_part.empty()) parts.add(current_part);
    return parts;
}

AST::StringArray AST::handleUnaryOperators(const StringArray& tokens) {
    StringArray processed;

    for (int i = 0; i < tokens.size; ++i) {
        processed.add(tokens.data[i]);
    }

    if (processed.size == 0) return processed;

    if (processed.data[0] == "-") processed.data[0] = "_NEG_";

    for (int i = 1; i < processed.size; ++i) {
        if (processed.data[i] == "-") {
            const string &prev_part = processed.data[i - 1];
            if (isOperator(prev_part) || prev_part == "(") {
                processed.data[i] = "_NEG_";
            }
        }
    }
    return processed;
}

void AST::infixToPostfix(const StringArray& tokens) {
    stack<string> opStack;
    postfixContainer.clear();
    for (int i = 0; i < tokens.size; ++i) {
        string part = tokens.data[i];

        if (!isOperator(part) && part != "(" && part != ")") {
            postfixContainer.add(part);
        } else if (part == "(") {
            opStack.push(part);
        } else if (part == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                postfixContainer.add(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        } else if (isOperator(part)) {
            while (!opStack.empty() && opStack.top() != "(" && getPrecedence(opStack.top()) >= getPrecedence(part)) {
                postfixContainer.add(opStack.top());
                opStack.pop();
            }
            opStack.push(part);
        }
    }
    while (!opStack.empty()) {
        if (opStack.top() == "(") throw runtime_error("Error: Unmatched left parenthesis.");
        postfixContainer.add(opStack.top());
        opStack.pop();
    }
}

AST::AST(string expression) : head(nullptr) {
    StringArray raw_tokens = tokenize(expression);
    StringArray processed_tokens = handleUnaryOperators(raw_tokens);
    infixToPostfix(processed_tokens);
    buildTree();

    raw_tokens.clear();
    processed_tokens.clear();
}

AST::~AST() {
    delete head;
    postfixContainer.clear();
}

void AST::buildTree() {
    if (head) { delete head; head = nullptr; }
    if (postfixContainer.size == 0) return;

    stack<BNode *> nodeStack;

    for (int i = 0; i < postfixContainer.size; ++i) {
        string part = postfixContainer.data[i];

        if (part == "_NEG_") {
            if (nodeStack.empty()) throw runtime_error("Missing operand for unary -");
            BNode *right = nodeStack.top(); nodeStack.pop();
            nodeStack.push(new BNode(part, nullptr, right));
        } else if (isOperator(part)) {
            if (nodeStack.size() < 2) throw runtime_error("Missing operands for " + part);
            BNode *right = nodeStack.top(); nodeStack.pop();
            BNode *left = nodeStack.top(); nodeStack.pop();
            nodeStack.push(new BNode(part, left, right));
        } else {
            nodeStack.push(new BNode(part));
        }
    }
    if (nodeStack.size() != 1) throw runtime_error("Error: Malformed expression.");
    head = nodeStack.top();
}

double AST::calculateOp(const string& op, double leftVal, double rightVal) {
    if (op == "+") return leftVal + rightVal;
    if (op == "-") return leftVal - rightVal;
    if (op == "*") return leftVal * rightVal;
    if (op == "/") {
        if (rightVal == 0) throw runtime_error("Div by zero");
        return leftVal / rightVal;
    }
    return 0;
}

string formatNumber(double val) {
    return format("{:.2f}", val);
}

int AST::getMaxDepth(BNode* node) {
    if (!node) return 0;
    if (!node->left && !node->right) return 1;

    int leftDepth = getMaxDepth(node->left);
    int rightDepth = getMaxDepth(node->right);
    return 1 + max(leftDepth, rightDepth);
}

bool AST::simplifyAtDepth(BNode* node, int currentDepth, int targetDepth) {
    if (!node) return false;

    bool changed = false;

    if (node->left) {
        bool childChanged = simplifyAtDepth(node->left, currentDepth + 1, targetDepth);
        if (childChanged) changed = true;
    }
    if (node->right) {
        bool childChanged = simplifyAtDepth(node->right, currentDepth + 1, targetDepth);
        if (childChanged) changed = true;
    }

    if (changed) return true;

    if (!isOperator(node->data)) return false;
    bool leftReady = (!node->left) || (!isOperator(node->left->data));
    bool rightReady = (!node->right) || (!isOperator(node->right->data));

    if (leftReady && rightReady) {
        int myDepthFromBottom = getMaxDepth(node);
        if (myDepthFromBottom == 2) {
             if (node->data == "_NEG_") {
                if (node->right) {
                    double val = -stod(node->right->data);
                    node->data = formatNumber(val);
                    delete node->right; node->right = nullptr;
                    return true;
                }
            } else {
                if (node->left && node->right) {
                    double l = stod(node->left->data);
                    double r = stod(node->right->data);
                    double res = calculateOp(node->data, l, r);
                    node->data = formatNumber(res);
                    delete node->left; node->left = nullptr;
                    delete node->right; node->right = nullptr;
                    return true;
                }
            }
        }
    }

    return false;
}

bool AST::simplifyLowestLevel() {
    if (!head || !isOperator(head->data)) return false;

    return simplifyAtDepth(head, 1, 0);
}

double AST::calculateRecursive(BNode *node) {
    if (!node) return 0.0;
    if (!isOperator(node->data)) return stod(node->data);
    if (node->data == "_NEG_") return -calculateRecursive(node->right);
    return calculateOp(node->data, calculateRecursive(node->left), calculateRecursive(node->right));
}

double AST::calculate() {
    if (!head) throw runtime_error("Tree not built.");
    return calculateRecursive(head);
}

AST::BNode *AST::getRoot() { return head; }