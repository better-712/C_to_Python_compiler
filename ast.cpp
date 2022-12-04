//
// Created by 10578 on 10/9/2019.
//

#include "ast.hpp"
#include <iostream>
#include <fstream>
namespace SPL {
    
    void print_ast(Node *node, int level) {
        if (node->type.compare("empty") == 0) {
            return;
        }

        //std::ofstream outfile;
        //outfile.open("out.txt",std::ios::app);
        
        if(!node->value.empty())
            std::cout << std::string(2 * level, ' ')<< node->type <<"  "<<node->value<< std::endl;
        else 
            std::cout << std::string(2 * level, ' ')<< node->type <<" ("<<std::to_string(node->line_no)<<")"<< std::endl;

        for (auto &child: node->children) {
            print_ast(child, level + 1);
        }
        
    }
    
    void visit_node(Node *node) {
        if (node->type.compare("empty") == 0) {
            return;
        }
        if (node->type.compare("Def") == 0) {
            std::vector<Node*> children=node->children;
            for(auto iter=children.begin();iter!=children.end();iter++)
            {
                if(iter==children.begin())
                    std::cout <<"Specifier: "<<iter->type<<std::endl;
                else print_ast(iter,0);
            }     
        }
        
        for (auto &child: node->children) {
            visit_node(child);
        }
        
    }
    
}
