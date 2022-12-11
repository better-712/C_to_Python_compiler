//
// Created by 10578 on 10/9/2019.
//
#include "type.cpp"

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
        Type *type;
    };
    class Variable_Entry : public Entry {
    public:
        Variable_Entry(std::string name, Type *type, int line_no){
            this->name = name;
            this->line_no = line_no;
            this->type = type;
        }
        
    };
    
    class Function_Entry : public Entry {
    public:
        std::vector<Type*> parameters;
        Function_Entry(Type* return_type, std::string id, int line_no, std::vector<Type*> parameters){
            this->parameters = parameters;
            this->name = id;
            this->line_no = line_no;
            this->type = return_type;
        }
        
    }; 
    
    
    class Struct_Def_Entry : public Entry{
        public:
            Struct_Type *struct_type;
            Struct_Def_Entry(Struct_Type *struct_type){
                this->struct_type=struct_type;
                this->name =struct_type->name;
                this->line_no = struct_type->line_no;
            }
    };
    

    
    
    std::unordered_map<std::string, Entry *> Symbol_Table;
    void insert(Entry *entry){
        std::cout <<"entry name: "<<entry->name<<std::endl;
        std::cout <<"entry type "<<entry->type->name<<std::endl;
        std::cout <<"entry line_no "<<entry->line_no<<std::endl;
        
        if (Symbol_Table.find(entry->name) != Symbol_Table.end()) {
            std::cout << "Semantic Error! " << entry->name << " has been in symbol table at line: " << entry->line_no<< std::endl;
        } else {
            Symbol_Table[entry->name] = entry;
        }
    }
    
    
    
//     std::vector<std::string>* get_Def(Node *node){
//         std::cout <<"var: "<<node->children.front()->children.front()->children.front()->value<<std::endl;
        
//         auto *decs = new std::vector<Node *>{};
//         decs->push_back(node->children.front());
//         Node *list=node->children.back();
//         while(list->type.compare("DecList") == 0){
//             decs->push_back(list->children.front());
//             list=list->children.back();
//         }
//         auto *vars = new std::vector<std::string>{};
//         for(auto iter=decs->begin();iter!=decs->end();iter++)
//             vars->push_back((*iter)->children.front()->children.front()->value);
//         return vars;      
//     }
    
    std::vector<Node *>* list_to_element(Node *node){
        auto *decs = new std::vector<Node *>{};
        decs->push_back(node->children.front());
        Node *list=node->children.back();
        while(list->type.compare(node->type) == 0){
            decs->push_back(list->children.front());
            list=list->children.back();
        }
        return decs;
    }
    
    Function_Entry* get_fun_entry(Type* type,Node *node){
        Node* varlist=node->children[2];
        std::vector<Type*> var_type;
        if(varlist->type.compare("VarList") == 0){
            std::vector<Node*>  *params =list_to_element(varlist);
            for(auto iter=params->begin();iter!=params->end();iter++){
                Node * spec=(*iter)->children[0];
                Primitive_Type *specifier=new Primitive_Type(spec->children[0]->value);
                var_type.push_back(specifier);
            }
        }
        Function_Entry *fun= new Function_Entry(type,node->children[0]->value,node->children[0]->line_no,var_type);
        return fun;
    }
    
    
    void visit_node(Node *node) {
        if (node->type.compare("empty") == 0) {
            return;
        }
        if (node->type.compare("Def") == 0) {           
            std::vector<Node*> children=node->children;
            Primitive_Type *specifier=new Primitive_Type(children[0]->children[0]->value);
            
          
//             std::cout <<"Specifier: "<<children[0]->type<<std::endl;
            
//             std::cout<<var[1]<<std::endl;
            
//            std::vector<std::string> var=get_Def(children[1]);
            std::vector<Node*> *decs =list_to_element(children[1]);
            auto *var = new std::vector<std::string>{};
            for(auto iter=decs->begin();iter!=decs->end();iter++)
                var->push_back((*iter)->children.front()->children.front()->value);
            for(auto iter=var->begin();iter!=var->end();iter++){
                Variable_Entry *var=new Variable_Entry(*iter,specifier,children[0]->line_no);
                insert(var);
            }
            
        }
        if (node->type.compare("ExtDef") == 0) {
            std::vector<Node*> children=node->children;
//             std::string specifier=children[0]->type;
            Primitive_Type *specifier=new Primitive_Type(children[0]->children[0]->value);
            if(node->children[1]->type.compare("ExtDecList") == 0){
                
                std::vector<Node*>  *vars =list_to_element(children[1]);
                auto *var = new std::vector<std::string>{};
                
                for(auto iter=vars->begin();iter!=vars->end();iter++)
                    var->push_back((*iter)->children.front()->value);
                
                for(auto iter=var->begin();iter!=var->end();iter++){
                    Variable_Entry *var=new Variable_Entry(*iter,specifier,children[0]->line_no);
                    insert(var);
                }
                
            }
            if(children[1]->type.compare("FunDec") == 0){
                
                auto *func_entry = get_fun_entry(specifier,children[1]);
                insert(func_entry);
                
                if(children[1]->children[2]->type.compare("VarList") == 0){
                    std::vector<Node*>  *params =list_to_element(children[1]->children[2]);
                    auto *var = new std::vector<std::string>{};
                    for(auto iter=params->begin();iter!=params->end();iter++){
                        std::string name=(*iter)->children.back()->children.front()->value;
                        Node * spec=(*iter)->children[0];
                        Primitive_Type *specifier=new Primitive_Type(spec->children[0]->value);
                        //
                        Variable_Entry *var=new Variable_Entry(name,specifier,spec->line_no);
                        insert(var);
                    }
//                         var->push_back((*iter)->children.back()->children.front()->value);
                
//                     for(auto iter=var->begin();iter!=var->end();iter++){
//                         Variable_Entry *var=new Variable_Entry(*iter,specifier,children[0]->line_no);
//                         insert(var);
//                     }
                    
                }
                
            }
            if(children[0]->children[0]->type.compare("StructSpecifier") == 0){
                
            }
            
        }
        
        
        for (auto &child: node->children) {
            visit_node(child);
        }
        
    }
    
    
}
