#include <iostream>
#include "AST.h"

int main() {
    try {
        std::string expr = "5 - - - 3";
        AST ast(expr);
        double result = ast.calculate();
        std::cout << "Expression: " << expr << std::endl;
        std::cout << "Result: " << result << std::endl;
        
        if (result == 2.0) {
            std::cout << "Test Passed" << std::endl;
        } else {
            std::cout << "Test Failed" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}