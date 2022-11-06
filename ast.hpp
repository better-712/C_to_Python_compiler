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
        std::string type;
        std::string value;
        std::vector<Node*> children;
        
        Node(std::string type){this->type=type;}
        Node(std::string type, Scan_Info *info) : type{type} {
        line_no = info->line_no;
        value = info->lexeme;
        }
    };
    
    void print_ast(Node *node, int level = 0);
}
    
#endif //__AST_HPP__
