#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

using namespace std;

class AST {
public:
    class BNode {
    public:
        string data;
        BNode* left;
        BNode* right;
        BNode(string val, BNode* l = nullptr, BNode* r = nullptr);
        ~BNode();
    };

private:
    BNode* head;
    vector<string> postfixContainer;

    int getPrecedence(const string& op);
    bool isOperator(const string& part);
    double calculateRecursive(BNode* node);

    vector<string> tokenize(const string& expression);
    vector<string> handleUnaryOperators(const vector<string>& tokens);
    void infixToPostfix(const vector<string>& tokens);

    BNode* copyTree(BNode* node);
    double calculateOp(const string& op, double left, double right);

    int getMaxDepth(BNode* node);
    bool simplifyAtDepth(BNode* node, int currentDepth, int targetDepth);

public:
    AST(string expression);
    ~AST();

    AST(const AST& other);
    AST& operator=(const AST& other);

    void buildTree();
    double calculate();
    BNode* getRoot();

    bool simplifyLowestLevel();
};

#endif