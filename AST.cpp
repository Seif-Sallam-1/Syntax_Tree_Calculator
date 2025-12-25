#include "AST.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <cmath>
#include <iomanip> // For formatting

using namespace std;

// --- BNode ---
AST::BNode::BNode(string val, BNode *l, BNode *r)
    : data(val), left(l), right(r) {}

AST::BNode::~BNode() {
    delete left;
    delete right;
}

// --- Deep Copy Logic ---
AST::AST(const AST& other) : head(nullptr) {
    if (other.head) {
        head = copyTree(other.head);
    }
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

// --- Helpers ---
int AST::getPrecedence(const string &op) {
    if (op == "_NEG_") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

bool AST::isOperator(const string &part) {
    return part == "+" || part == "-" || part == "*" || part == "/" || part == "_NEG_";
}

// --- Parsing ---
vector<string> AST::tokenize(const string& expression) {
    vector<string> parts;
    string current_part;
    for (char c : expression) {
        if (isspace(c)) {
            if (!current_part.empty()) {
                parts.push_back(current_part);
                current_part.clear();
            }
            continue;
        }
        string op_str(1, c);
        if (op_str == "+" || op_str == "-" || op_str == "*" ||
            op_str == "/" || op_str == "(" || op_str == ")") {
            if (!current_part.empty()) {
                parts.push_back(current_part);
                current_part.clear();
            }
            parts.push_back(op_str);
        } else {
            current_part += c;
        }
    }
    if (!current_part.empty()) parts.push_back(current_part);
    return parts;
}

vector<string> AST::handleUnaryOperators(const vector<string>& tokens) {
    vector<string> processed = tokens;
    if (processed.empty()) return processed;
    if (processed[0] == "-") processed[0] = "_NEG_";
    for (size_t i = 1; i < processed.size(); ++i) {
        if (processed[i] == "-") {
            const string &prev_part = processed[i - 1];
            if (isOperator(prev_part) || prev_part == "(") {
                processed[i] = "_NEG_";
            }
        }
    }
    return processed;
}

void AST::infixToPostfix(const vector<string>& tokens) {
    stack<string> opStack;
    postfixContainer.clear();
    for (const string &part: tokens) {
        if (!isOperator(part) && part != "(" && part != ")") {
            postfixContainer.push_back(part);
        } else if (part == "(") {
            opStack.push(part);
        } else if (part == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                postfixContainer.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        } else if (isOperator(part)) {
            while (!opStack.empty() && opStack.top() != "(" &&
                   getPrecedence(opStack.top()) >= getPrecedence(part)) {
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

// --- Constructor ---
AST::AST(string expression) : head(nullptr) {
    vector<string> raw_tokens = tokenize(expression);
    vector<string> processed_tokens = handleUnaryOperators(raw_tokens);
    infixToPostfix(processed_tokens);
    buildTree();
}

AST::~AST() { delete head; }

void AST::buildTree() {
    if (head) { delete head; head = nullptr; }
    if (postfixContainer.empty()) return;

    stack<BNode *> nodeStack;
    for (const string &part: postfixContainer) {
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

// --- Step-by-Step Logic ---

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

// Format to remove trailing zeros (e.g., 5.0000 -> 5)
string formatNumber(double val) {
    stringstream ss;
    ss << val;
    return ss.str();
}

bool AST::simplifyOneStep() {
    if (!head) return false;
    // If root is not an operator, we are already fully simplified
    if (!isOperator(head->data)) return false;
    return simplifyRecursive(head);
}

bool AST::simplifyRecursive(BNode* node) {
    if (!node) return false;

    // 1. Try to simplify children first (Deepest first -> Post-Order Traversal)
    if (node->left && isOperator(node->left->data)) {
        if (simplifyRecursive(node->left)) return true;
    }
    if (node->right && isOperator(node->right->data)) {
        if (simplifyRecursive(node->right)) return true;
    }

    // 2. If children are ready (numbers), solve this node
    if (isOperator(node->data)) {
        // Case A: Unary Negation (e.g., -5)
        if (node->data == "_NEG_") {
            if (node->right && !isOperator(node->right->data)) {
                double val = -stod(node->right->data);
                node->data = formatNumber(val);
                delete node->right; node->right = nullptr;
                return true;
            }
        }
        // Case B: Binary Operation (e.g., 3 + 4)
        else {
            if (node->left && node->right &&
                !isOperator(node->left->data) && !isOperator(node->right->data)) {

                double l = stod(node->left->data);
                double r = stod(node->right->data);
                double res = calculateOp(node->data, l, r);

                node->data = formatNumber(res);

                // Remove children since they are now merged into this node
                delete node->left; node->left = nullptr;
                delete node->right; node->right = nullptr;
                return true;
            }
        }
    }
    return false;
}

// --- Standard Calculation ---
double AST::calculateRecursive(BNode *node) {
    if (!node) return 0.0;
    if (!isOperator(node->data)) {
        try { return stod(node->data); }
        catch (...) { throw runtime_error("Error: Invalid operand '" + node->data + "'"); }
    }
    if (node->data == "_NEG_") return -calculateRecursive(node->right);

    double leftVal = calculateRecursive(node->left);
    double rightVal = calculateRecursive(node->right);
    return calculateOp(node->data, leftVal, rightVal);
}

double AST::calculate() {
    if (!head) throw runtime_error("Tree not built.");
    return calculateRecursive(head);
}

AST::BNode *AST::getRoot() { return head; }