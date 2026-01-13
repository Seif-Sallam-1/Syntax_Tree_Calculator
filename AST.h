#ifndef AST_H
#define AST_H

#include <string>

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

    struct StringArray {
        string* data;
        int size;

        StringArray() : data(nullptr), size(0) {}

        void add(string s);
        void clear();
    };

private:
    BNode* head;

    StringArray postfixContainer;

    int getPrecedence(const string& op);
    bool isOperator(const string& part);
    double calculateRecursive(BNode* node);

    StringArray tokenize(const string& expression);
    StringArray handleUnaryOperators(const StringArray& tokens);
    void infixToPostfix(const StringArray& tokens);

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