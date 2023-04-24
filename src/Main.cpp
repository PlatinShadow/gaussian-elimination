#include <iostream>
#include <string>

#include "Interpreter.h"

int main() {
    std::cout << "Linear Equation Solver v0.1" << std::endl;
    std::cout << "Using Gausian Elimination Method" << std::endl << std::endl;
    std::cout << "Enter Linear Equation System, s to solve" << std::endl;
    
    LES::Interpreter interpreter;

    std::string line;
    while(std::getline(std::cin, line)) {
        
        if(line.length() == 1) {
            switch (line[0]) {
            case 's':
                break;
           
            case 'q':
                return EXIT_SUCCESS;

            default: 
                std::cout << "Unknown command. s to solve, q to quit" << std::endl; 
                continue;
            }
        }

        if(interpreter.processLine(line) == false) {
            std::cout << "Syntax Error" << std::endl;
        }
    }

    return EXIT_SUCCESS;
}