#include <iostream>
#include <string>

#include "Interpreter.h"

int main() {
    std::cout << "Linear Equation Solver v0.1" << std::endl;
    std::cout << "Using Gausian Elimination Method" << std::endl << std::endl;
    std::cout << "Enter Linear Equation System, s to solve" << std::endl;
    
    LES::Interpreter interpreter;

    std::string line;
    
    std::vector<std::vector<LES::Term>> terms; 
    
    while(std::getline(std::cin, line)) {
        if(line.length() == 1) {
            switch (line[0]) {
            case 's': {
                
                 for(auto term : terms) {
                    for(auto x : term) {
                        std::cout << (x.Scalar > 0 ? "+" : "") << x.Scalar << x.Literal;
                    } 
                    std::cout << std::endl;
                 }

                } break;
           
            case 'q':
                return EXIT_SUCCESS;

            default: 
                std::cout << "Unknown command. s to solve, q to quit" << std::endl; 
                continue;
            }
        }

        auto result = interpreter.processLine(line);

        if(result.has_value()) {
            terms.push_back(result.value());
        }
    }

    return EXIT_SUCCESS;
}