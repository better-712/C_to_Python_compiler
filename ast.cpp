//
// Created by 10578 on 10/9/2019.
//

#include "ast.hpp"

namespace SPL {
    
    Node::Node(string type) : type{type} {}
    
    Node::Node(string type, Scan_Info *info) : type{type} {
        line_no = info->line_no;
        std::string lexeme = info->lexeme;
        switch (this->leaf_type) {
            case token::INT:
                value = (unsigned int)std::stoul(lexeme, nullptr, 0);
                break;
            case token::FLOAT:
                value = std::stof(lexeme, nullptr);
                break;
            case token::CHAR:
                value = trim(lexeme, "'").c_str()[0];
                break;
            default:
                break;
        }
    }
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
