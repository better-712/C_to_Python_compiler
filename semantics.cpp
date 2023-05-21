#include <iostream>
#include <vector>
#include "type.cpp"
#include "ast.hpp"
#include <string.h>
#include "symbol.hpp"
namespace SPL {
  //INT FLOAT CHAR ARRAY  STRUCT
  extern int INT,FLOAT,CHAR,STRUCT,ARRAY;
  
  Symbol_Table *cur_table=new Symbol_Table;
  Symbol_Type cur_specifier;
  
  std::vector<Node *>* list_to_e(Node *node){
        auto *decs = new std::vector<Node *>{};
        decs->push_back(node->children.front());
        Node *list=node->children.back();
        while(list->type.compare(node->type) == 0){
            decs->push_back(list->children.front());
            list=list->children.back();
        }
        return decs;
    }
  
  void enter_scope(){
    Symbol_Table *top=new Symbol_Table;
    top->next=cur_table;
    cur_table=top;
    printf("enter_scope\n");
  }
  void pop_scope(){
    cur_table=cur_table->next;
    printf("pop_scope\n");
  }
  
  void record_Spec(Node* tree){
    //to do for struct
    
    if(tree->children[0]->type.compare("TYPE")==0){
      
      //cur_specifier.id=(char*)(tree->children[0]->value).c_str();
      if(tree->children[0]->value.compare("int")==0)
        cur_specifier.type=INT;
      if(tree->children[0]->value.compare("float")==0)
        cur_specifier.type=FLOAT;
      if(tree->children[0]->value.compare("char")==0)
        cur_specifier.type=CHAR;
     // std::cout<<cur_spec.id<<std::endl;
    }else{
//todo
      //StructSpecifier
      //STRUCT ID
      
      cur_specifier.type=STRUCT;
      //cur_spec.id=cgen_ID(tree->children[0]->children[1]);
     // std::cout<<"cur_spec.id:"<<cur_spec.id<<"cur_spec.type:"<<cur_spec.type<<std::endl;
    }
    
  }
  
  void eliminate_Spec(){
    cur_specifier = Symbol_Type();
  }
  
  Symbol_Type analyze_Int (Node* tree) {
    Symbol_Type res;
    res.type=INT;
    return res;
  }
  
  Symbol_Type analyze_FLOAT (Node* tree) {
    Symbol_Type res;
    res.type=FLOAT;
    return res;
  }
  
  Symbol_Type analyze_CHAR (Node* tree) {
    Symbol_Type res;
    res.type=CHAR;
    return res;
  }
  
  
  int analyze_Specifier_FunDec_CompSt (Node* tree){
    printf("analyze_Specifier_FunDec_CompSt\n");
    
    return 0;
  }
  
  Symbol_Type analyze_Exp(Node *tree) {
    if (tree->children[0]->type.compare("INT") == 0)
        return analyze_Int(tree->children[0]);
//     if (tree->children[0]->type.compare("ID") == 0)
//         return cgen_ID(tree->children[0]);
    if (tree->children[0]->type.compare("FLOAT") == 0)
        return analyze_FLOAT(tree->children[0]);
    if (tree->children[0]->type.compare("CHAR") == 0)
        return analyze_CHAR(tree->children[0]);
  }
  
  void analyze_VarDec(Node *tree) {
    printf("analyze_VarDec\n");
    
    int line_no=tree->children[0]->line_no;
    Symbol* a=new Symbol;
    a->line_no=line_no;
    a->symbol_type.type=cur_specifier.type;
    a->symbol_type.tag=cur_specifier.tag;
    //only used in function.   to do:must follow by=
    if(tree->children.size() == 3){
      a->name=tree->children[0]->children[0]->value;
      a->symbol_type.type=ARRAY;
      a->symbol_type.size=-1;
      //printf("size -1\n");
    }else if(tree->children.size() == 4){
      a->name=tree->children[0]->children[0]->value;
      a->symbol_type.type=ARRAY;
      a->symbol_type.size=std::stoi(tree->children[2]->value);
     // printf("size %d\n",a->size);
    }else{
      a->name=tree->children[0]->value;
    }
   
    //std::cout<<"name:"<<name<<std::endl;
    if(cur_table->table.count(a->name) != 0){
      std::cout<<"VariableRedefined:"<<a->name<<std::endl;
    }else
      cur_table->table[a->name]=a;
  
  }
  
  void analyze_ExtDecList(Node *tree) {
    printf("analyze_ExtDecList\n");
    analyze_VarDec(tree->children[0]);
    if(tree->children.size() == 3){
      analyze_ExtDecList(tree->children[2]);
    }
  }
  
  void extDefVisit_SES_PureType(Node *tree) {    
    //Node *extDecList = tree->children[1];
    record_Spec(tree->children[0]);
    analyze_ExtDecList(tree->children[1]);  
    //std::cout<<"cur_specifier.type: "<<cur_specifier.type<<std::endl;
    eliminate_Spec();
    //std::cout<<"cur_specifier.type: "<<cur_specifier.type<<std::endl;
    //std::cout<<"FLOAT: "<<FLOAT<<std::endl;
    
}
  void extDefVisit_SES_StructType(Node *node) {
//     string structName = std::get<string>(node->get_nodes(0, 0, 1)->value);
//     Node *extDefList = node->get_nodes(1);
//     string variableName = getStrValueFromExtDecList(extDefList);
//     extDefVisit_SS(node);
//     if (symbolTable.count(structName) == 0) {
//         structNoDefinition(std::get<int>(node->value),structName);
//         // but this do not need to print, it use to happen is extDefVisit_SS;
//         // do not match there
//         //structRedefined(std::get<int>(node->value), structName);
//     } else {
//         do {
//             if (symbolTable.count(variableName) != 0) {
//                 variableRedefined(std::get<int>(node->value), variableName);
//             }
//             if (extDefList->get_nodes(0)->nodes.size() == 1) {
//                 //Struct with variable definition
//                 symbolTable[variableName] = symbolTable[structName];
//             } else {
//                 //Struct with variable definition - with Array
//                 symbolTable[variableName] = new Type(variableName, CATEGORY::ARRAY,
//                                                      getArrayFromVarDec(extDefList->get_nodes(0),
//                                                                         symbolTable[structName]));
//             }
//             if (extDefList->nodes.size() == 1) {
//                 return;
//             }
//             extDefList = extDefList->get_nodes(2);
//             variableName = getStrValueFromExtDecList(extDefList);
//         } while (true);
//     }
}

  
  int analyze_Specifier_ExtDecList_SEMI (Node* tree){
    printf("analyze_Specifier_ExtDecList_SEMI\n");
    if (tree->children[0]->children[0]->type.compare("TYPE") == 0) {
        // global puretype variables
        extDefVisit_SES_PureType(tree);
    } else {
        //global struct def and varis
        extDefVisit_SES_StructType(tree);
        
    }
    return 0;
  }
  
  void analyze_Dec (Node* tree){
    printf("analyze_Dec\n");
    analyze_VarDec(tree->children[0]);
    Symbol_Type exp=analyze_Exp(tree->children[2]);
    std::cout<<"exp:"<<exp->type<<std::endl;
  }
  
  void analyze_DecList (Node* tree){
    printf("analyze_DecList\n");
    analyze_Dec(tree->children[0]);
    if(tree->children.size() == 3){
      analyze_DecList(tree->children[2]);
    }
  }
  
  void analyze_Def (Node* tree){
    printf("analyze_Def\n");
    //std::cout<<"tree:"<<tree->type<<std::endl;
    
    record_Spec(tree->children[0]);
    analyze_DecList(tree->children[1]);
    eliminate_Spec();
  }
  
  void analyze_DefList (Node* tree){
    printf("analyze_DefList\n");
    //std::cout<<"tree:"<<tree->type<<std::endl;
    
    if(tree->type.compare("empty") == 0)return;
    analyze_Def(tree->children[0]);
    analyze_DefList(tree->children[1]);
    return;    
  }
  
  int analyze_StructSpecifier (Node* tree){
    printf("analyze_StructSpecifier\n");
    int result=0;
    
    std::string name=tree->children[1]->value;
    int line_no=tree->children[1]->line_no;
    //std::cout<<"name:"<<name<<std::endl;
    
    //STRUCT ID LC DefList RC  DefList : Def DefList
    if(tree->children.size()==5){
      Symbol* a=new Symbol;
      a->name=name;
      a->symbol_type.tag=name;
      a->line_no=line_no;
      
      //enter scope
      enter_scope();
      
      analyze_DefList(tree->children[3]);
      
      pop_scope();
      
      cur_table->insert(a);
      
    }else if(tree->children.size()==3)//STRUCT ID  to do:record
    {
                           
    }
    
    return result;
  }
  
  int analyze_ExtDef (Node* tree){
    //printf("cgen_Line\n");
    int result=0;
    
    Node *tmp=tree->children[1];
    if(tmp->type.compare("ExtDecList") == 0)
      result = result+analyze_Specifier_ExtDecList_SEMI(tree);
    else if(tmp->type.compare("SEMI") == 0)
      result = result+analyze_StructSpecifier(tree->children[0]->children[0]);
    else if(tmp->type.compare("FunDec") == 0)
      result = result+analyze_Specifier_FunDec_CompSt(tree);
    
    return result;
  }
  
  int analyze_Program (Node* tree){
    int result=0;
    
    Node *ExtDefList=tree->children[0];
    std::vector<Node *>* list=list_to_e(ExtDefList);
    
    
    for(auto iter=list->begin();iter!=list->end();iter++){
        result = result+analyze_ExtDef((*iter));
        
    }
    
    printf("analyze_Program\n");
    return result;   
  }
  
  int semantic_analyze (Node *root){
    return analyze_Program(root);
  }
    
}
