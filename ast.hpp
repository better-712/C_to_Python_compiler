//
// Created by 10578 on 10/9/2019.
//

#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <variant>
#include "parser.tab.hpp"
#include "utils.hpp"
#include "scanner.hpp"

namespace SPL {
    /* Class pre-definition */

    class Node;
    
    using token = SPL_Parser::token;
    
    class Node {
    public:
        int line_no = INT32_MAX;
        string type;
        std::variant<unsigned int, float, char> value;
        vector<Node*> children;
        
        Node(string type){this.type=type;};
        Node(string type, Scan_Info *info) : type{type} {
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
        
    };
    
    void print_ast(Node *node, int level = 0);
    
#endif //__AST_HPP__
