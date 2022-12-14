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
            
            Struct_Def_Entry(std::string id,Struct_Type *struct_type){
                this->type=struct_type;
                this->name =id;
//                 this->line_no ;
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
    
    Type* get_type(Node* node){
        Type *specifier;
        if(node->type.compare("TYPE") == 0){
            specifier=new Primitive_Type(node->value);
        } 
        if(node->type.compare("StructSpecifier") == 0){
            std::cout <<"size : "<<node->children.size()<<std::endl;
            if(node->children.size()==2){
                
            }
            else{
                Node* deflist=node->children[3];
                std::vector<std::pair<std::string, Type *>> member_vector;
                std::vector<Node *>* def_list=list_to_element(deflist);
                for(auto iter=def_list->begin();iter!=def_list->end();iter++){
                    Type *specifier=get_type((*iter)->children[0]->children[0]);
                    Node* declist=(*iter)->children[1];
                    std::vector<Node *>* dec_list=list_to_element(declist);
                
                    for(auto iter=dec_list->begin();iter!=dec_list->end();iter++){
                        std::string var=(*iter)->children.front()->children.front()->value;
                        member_vector.push_back(make_pair(var,specifier));
                    }
                }
                specifier=new Struct_Type(member_vector);
                
            }           
                
        }
          
        return specifier;
    }
//     std::pair<Type *, std::vector<VarDec_Node *> *> Local_Resolver::get_info(Def_Node *node) {
//         auto specifier_type = get_type(node->specifier);
//         auto *decs = new std::vector<VarDec_Node *>{};

//         DecList_Node *dec_list = node->dec_list;
//         while (dec_list) {
//             decs->push_back(dec_list->dec->var_dec);
//             dec_list = dec_list->dec_list;
//         }

//         return make_pair(specifier_type, decs);
//     }
    
    void visit(Node *node){
        for (auto &child: node->children) {
            visit_node(child);
        }
    }
    
    void visit_node(Node *node) {
        if (node->type.compare("empty") == 0) {
            return;
        }
        if (node->type.compare("Def") == 0) {           
            std::vector<Node*> children=node->children;
            Primitive_Type *specifier=new Primitive_Type(children[0]->children[0]->value);
            
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
            if(children[1]->type.compare("ExtDecList") == 0){
                
                std::vector<Node*>  *vars =list_to_element(children[1]);
                auto *var = new std::vector<std::string>{};
                
                for(auto iter=vars->begin();iter!=vars->end();iter++)
                    var->push_back((*iter)->children.front()->value);
                
                for(auto iter=var->begin();iter!=var->end();iter++){
                    Variable_Entry *var=new Variable_Entry(*iter,specifier,children[0]->line_no);
                    insert(var);
                }
                return;
                
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
                          
                }
                visit(children[1]);
            }
            if(children[0]->children[0]->type.compare("StructSpecifier") == 0){
                Type* s=get_type(children[0]->children[0]);
                std::string name=children[0]->children[0]->children[1]->value;
                auto *symbol = new Struct_Def_Entry(name,dynamic_cast<Struct_Type*>(s));
                insert(symbol);
                return;
            }
            
        }
        
        
        for (auto &child: node->children) {
            visit_node(child);
        }
        
    }
    
    
}
