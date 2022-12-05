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
    
        
    class Entry {
    public:
        std::string name;
        int line_no = -1;
    };
    class Variable_Entry : public Entry {
    public:
        Variable_Symbol(std::string name, std::string name type, int line_no){
        this->name = name;
        this->line_no = type->line_no;
        this->type = type;
    }

        std::string type;
    };
    
    
    std::unordered_map<std::string, Entry *> Symbol_Table;
    void insert(Entry *entry){
        std::cout <<"entry name: "<<entry->name<<std::endl;
        std::cout <<"entry type "<<entry->type<<std::endl;
        std::cout <<"entry line_no "<<entry->line_no<<std::endl;
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
        auto *vars = new std::vector<std::string>{};
        for(auto iter=decs->begin();iter!=decs.end();iter++)
            vars->push_back((*iter)->children.front()->children.front()->value);
        return *vars;      
    }
    
    
    void visit_node(Node *node) {
        if (node->type.compare("empty") == 0) {
            return;
        }
        if (node->type.compare("Def") == 0) {
            std::vector<Node*> children=node->children;
            std::string specifier=children[0]->type;
            std::cout <<"Specifier: "<<children[0]->type<<std::endl;
            std::vector<std::string>var=get_Def(children[1]);
            std::cout<<var[1]<<std::endl;
            for(auto iter=var->begin();iter!=var.end();iter++){
                Variable_Entry *var=new Variable_Entry(*iter,specifier,children[0]->line_no);
                insert(var);
            }
            
            
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
