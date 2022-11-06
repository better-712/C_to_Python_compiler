//
// Created by 10578 on 10/9/2019.
//

#include "ast.hpp"

namespace SPL {
    
    void print_ast(Node *node, int level) {
        std::cout << node->type<<std::endl;
//         if (node->is_empty()) {
//             return;
//         }

//         std::cout << std::string(2 * indent_level, ' ') // Indent format
//                   << node->to_string() << std::endl;

//         for (auto &child: node->get_child()) {
//             print_ast(child, indent_level + 1);
//         }
        
    }
    
}
