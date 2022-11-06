//
// Created by 10578 on 10/9/2019.
//

#include "ast.hpp"

namespace SPL {
    
    void print_ast(Node *node, int level) {
        if (node->type.compare("empty") == 0) {
            return;
        }

        std::cout << std::string(2 * level, ' ')<< node->type <<" ("<<std::to_string(node->line_no)<<")"<< std::endl;

        for (auto &child: node->children) {
            print_ast(child, level + 1);
        }
        
    }
    
}
