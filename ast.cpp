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
    
        
    
    std::unordered_map<std::string, std::string> Symbol_Table;
    void insert(Entry *entry){
    }
    
    
    std::vector<std::string> get_Def(Node *node){
        auto *decs = new std::vector<Node *>{};
        decs->push_back(node->children.front());
        std::cout <<"var: "<<node->children.front()->children.front()->children.front()->value<<std::endl;
        Node *list=node->children.back();
        while(list->type.compare("DecList") == 0){
            decs->push_back(list->children.front());
            list=list->children.back();
        }
        auto vars = new std::vector<std::string>{};
        for(auto iter=decs.begin();iter!=decs.end();iter++)
            vars.push_back((*iter)->children.front()->children.front()->value)
        return vars      
    }
    
    
    void visit_node(Node *node) {
        if (node->type.compare("empty") == 0) {
            return;
        }
        if (node->type.compare("Def") == 0) {
            std::vector<Node*> children=node->children;
            std::cout <<"Specifier: "<<children[0]->type<<std::endl;
            std::vector<std::string>var=get_Def(children[1]);
            std::cout<<var[1]<<std::endl;
//             for(auto iter=children.begin();iter!=children.end();iter++)
//             {
//                 if(iter==children.begin())
//                     std::cout <<"Specifier: "<<(*iter)->type<<std::endl;
//                 else 
//                     print_ast(iter,0);
//             }     
        }
        
        for (auto &child: node->children) {
            visit_node(child);
        }
        
    }
    
    
}
