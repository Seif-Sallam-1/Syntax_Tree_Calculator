#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

class AST {
public:
    class BNode {
    public:
        std::string data;
        BNode* left;
        BNode* right;
        BNode(std::string val, BNode* l = nullptr, BNode* r = nullptr);
        ~BNode();
    };

private:
    BNode* head;
    std::vector<std::string> postfixContainer;

    // Helper functions
    int getPrecedence(const std::string& op);
    bool isOperator(const std::string& part);
    double calculateRecursive(BNode* node);

    // Parsing pipeline
    std::vector<std::string> tokenize(const std::string& expression);
    std::vector<std::string> handleUnaryOperators(const std::vector<std::string>& tokens);
    void infixToPostfix(const std::vector<std::string>& tokens);

    // New Helpers for Step-by-Step
    BNode* copyTree(BNode* node);
    bool simplifyRecursive(BNode* node);
    double calculateOp(const std::string& op, double left, double right);

public:
    AST(std::string expression);
    ~AST();

    // Copy Constructor & Assignment Operator (Crucial for History)
    AST(const AST& other);
    AST& operator=(const AST& other);

    void buildTree();
    double calculate();
    BNode* getRoot();

    // New Feature: Performs ONE calculation step
    bool simplifyOneStep();
};

#endif