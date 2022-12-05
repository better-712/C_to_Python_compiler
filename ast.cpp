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
        std::string type;
    };
    class Variable_Entry : public Entry {
    public:
        Variable_Entry(std::string name, std::string type, int line_no){
        this->name = name;
        this->line_no = line_no;
        this->type = type;
    }
        
    };
    
    
    std::unordered_map<std::string, Entry *> Symbol_Table;
    void insert(Entry *entry){
        std::cout <<"entry name: "<<entry->name<<std::endl;
        std::cout <<"entry type "<<entry->type<<std::endl;
        std::cout <<"entry line_no "<<entry->line_no<<std::endl;
        
        if (Symbol_Table.find(entry->name) != Symbol_Table.end()) {
            std::cout << "Semantic Error! " << entry->name << " has been in symbol table at line: " << entry->line_no<< std::endl;
        } else {
            Symbol_Table[entry->name] = entry;
        }
    }
    
    
    
    std::vector<std::string> get_Def(Node *node){
        std::cout <<"var: "<<node->children.front()->children.front()->children.front()->value<<std::endl;
        
        auto *decs = new std::vector<Node *>{};
        decs->push_back(node->children.front());
        Node *list=node->children.back();
        while(list->type.compare("DecList") == 0){
            decs->push_back(list->children.front());
            list=list->children.back();
        }
        auto *vars = new std::vector<std::string>{};
        for(auto iter=decs->begin();iter!=decs->end();iter++)
            vars->push_back((*iter)->children.front()->children.front()->value);
        return *vars;      
    }
    
    
    void visit_node(Node *node) {
        if (node->type.compare("empty") == 0) {
            return;
        }
        if (node->type.compare("Def") == 0) {
            std::cout <<"Specifier: "<<children[0]->type<<std::endl;
            
            std::vector<Node*> children=node->children;
            std::string specifier=children[0]->type;
            std::vector<std::string>var=get_Def(children[1]);
            
//             std::cout<<var[1]<<std::endl;
            for(auto iter=var.begin();iter!=var.end();iter++){
                Variable_Entry *var=new Variable_Entry(*iter,specifier,children[0]->line_no);
                insert(var);
            }
            
        }
        if (node->type.compare("ExtDef") == 0) {
            std::vector<Node*> children=node->children;
            std::string specifier=children[0]->type;
            if(node->children[1]->type.compare("ExtDecList") == 0){
                
                auto *decs = new std::vector<Node *>{};
                decs->push_back(node->children.front());
                Node *list=node->children.back();
                while(list->type.compare("ExtDecList") == 0){
                    decs->push_back(list->children.front());
                    list=list->children.back();
                }
                auto *vars = new std::vector<std::string>{};
                for(auto iter=decs->begin();iter!=decs->end();iter++)
                    vars->push_back((*iter)->children.front()->value);
                
                for(auto iter=var.begin();iter!=var.end();iter++){
                    Variable_Entry *var=new Variable_Entry(*iter,specifier,children[0]->line_no);
                    insert(var);
                }
                
            }
            
        }
        
        
        for (auto &child: node->children) {
            visit_node(child);
        }
        
    }
    
    
}
