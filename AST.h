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

    // Helpers
    int getPrecedence(const std::string& op);
    bool isOperator(const std::string& part);
    double calculateRecursive(BNode* node);

    // Parsing
    std::vector<std::string> tokenize(const std::string& expression);
    std::vector<std::string> handleUnaryOperators(const std::vector<std::string>& tokens);
    void infixToPostfix(const std::vector<std::string>& tokens);

    // Step-by-Step Helpers
    BNode* copyTree(BNode* node);
    double calculateOp(const std::string& op, double left, double right);

    // CHANGED: Returns current max depth of the tree
    int getMaxDepth(BNode* node);
    // CHANGED: Simplifies only nodes at a specific target depth
    bool simplifyAtDepth(BNode* node, int currentDepth, int targetDepth);

public:
    AST(std::string expression);
    ~AST();

    AST(const AST& other);
    AST& operator=(const AST& other);

    void buildTree();
    double calculate();
    BNode* getRoot();

    // Renamed for clarity, though you can keep it 'simplifyOneStep'
    bool simplifyLowestLevel();
};

#endif