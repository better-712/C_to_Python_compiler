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
        Node(std::string type, Node *n1) : type{type} {
        children.push_back(n1);
        }
        Node(std::string type, Node *n1,Node *n2) : type{type} {
        children.push_back(n1);
        children.push_back(n2);
        }
        Node(Node *n0, Node *n1,Node *n2) : {
        std::cout<<"????????"<<std::endl;
        }
        Node(std::string type, Node *n1,Node *n2,Node *n3) : type{type} {
        children.push_back(n1);
        children.push_back(n2);
        children.push_back(n3);
        }
        Node(std::string type, Node *n1,Node *n2,Node *n3,Node *n4) : type{type} {
        children.push_back(n1);
        children.push_back(n2);
        children.push_back(n3);
        children.push_back(n4);
        }
        Node(std::string type, Node *n1,Node *n2,Node *n3,Node *n4,Node *n5) : type{type} {
        children.push_back(n1);
        children.push_back(n2);
        children.push_back(n3);
        children.push_back(n4);
        children.push_back(n5);
        }
        Node(std::string type, Node *n1,Node *n2,Node *n3,Node *n4,Node *n5,Node *n6) : type{type} {
        children.push_back(n1);
        children.push_back(n2);
        children.push_back(n3);
        children.push_back(n4);
        children.push_back(n5);
        children.push_back(n6);
        }
        Node(std::string type, Node *n1,Node *n2,Node *n3,Node *n4,Node *n5,Node *n6,Node *n7) : type{type} {
        children.push_back(n1);
        children.push_back(n2);
        children.push_back(n3);
        children.push_back(n4);
        children.push_back(n5);
        children.push_back(n6);
        children.push_back(n7);
        }
    };
    
    void print_ast(Node *node, int level = 0);
}
    
#endif //__AST_HPP__
