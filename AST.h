#ifndef AST_H
#define AST_H

#include <string>
#include <vector>


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

    int getPrecedence(const std::string& op);
    bool isOperator(const std::string& part);
    double calculateRecursive(BNode* node);

    std::vector<std::string> tokenize(const std::string& expression);
    std::vector<std::string> handleUnaryOperators(const std::vector<std::string>& tokens);
    void infixToPostfix(const std::vector<std::string>& tokens);

public:
    AST(std::string expression);
    ~AST();


    void buildTree();
    double calculate();
    BNode* getRoot();
};

#endif