#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

using namespace std;

class AST {
public:
    // Nested Class BNode
    class BNode {
    public:
        string data;
        BNode* left;
        BNode* right;

        // Constructor
        BNode(string val, BNode* l = nullptr, BNode* r = nullptr);

        // Destructor
        ~BNode();
    };

private:
    BNode* head;
    vector<string> postfixContainer;

    // Helper functions
    int getPrecedence(const string& op);
    bool isOperator(const string& part);

    double calculateRecursive(BNode* node);

public:
    // Constructor
    AST(string expression);

    // Destructor
    ~AST();

    // Core Functions
    void buildTree();
    double calculate();

    // Accessor for GUI
    BNode* getRoot();
};

#endif // AST_H