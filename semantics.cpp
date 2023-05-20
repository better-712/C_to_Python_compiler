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
  
  int analyze_Specifier_FunDec_CompSt (Node* tree){
    printf("analyze_Specifier_FunDec_CompSt\n");
    
    return 0;
  }
  
      //     Node *extDecList = node->get_nodes(1);
//     string name = getStrValueFromExtDecList(extDecList);
//     auto _type = snt[std::get<string>(node->get_nodes(0, 0)->value)];
//     do {
//         if (symbolTable.count(name) != 0) {
//             variableRedefined(std::get<int>(node->value), name);
//         }
//         const auto &PrimitiveType = Type::getPrimitiveType(_type);
//         if (extDecList->get_nodes(0, 0)->nodes.empty()) {
//             symbolTable[name] = PrimitiveType;
//         } else {
//             symbolTable[name] = new Type(name, CATEGORY::ARRAY,
//                                          getArrayFromVarDec(extDecList->get_nodes(0),
//                                                             PrimitiveType));
//         }
//         if (extDecList->nodes.size() == 1) {
//             break;
//         }
//         extDecList = extDecList->get_nodes(2);
//         name = getStrValueFromExtDecList(extDecList);
//     } while (true);
  
  void analyze_VarDec(Node *tree) {
    printf("analyze_VarDec\n");
    if(tree->children.size() == 3)//size not sure
    {
//       char *id;
//       int l_id;
//       id = cgen_VarDec(tree->children[0]);
//       l_id= strlen(id);
//       result = (char*)calloc(l_id + 1, sizeof(char));
//       memcpy(result, id, l_id * sizeof(char));
      
    }else if(tree->children.size() == 4)
    {
      
//       //arr
//       cur_spec.type=ARRAY;
//       //value
//       cur_spec.id=in;
      
    }else{
      std::string name=tree->children[0]->value;
      int line_no=tree->children[0]->line_no;
      //std::cout<<"name:"<<name<<std::endl;
      if(cur_table->table.count(name) != 0){
        std::cout<<"VariableRedefined:"<<name<<std::endl;
      }else{
        Symbol* a=new Symbol;
        a->name=name;
        a->line_no=line_no;
        a->type=cur_specifier.type;
        a->tag=cur_specifier.tag;
        cur_table->table[name]=a;
      }     
      
    }
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
      a->line_no=line_no;
      cur_table->insert(a);
      
    }else if(tree->children.size()==3)//STRUCT ID
    {
      
//       char * DefList;
//       int l_DefList;
//       DefList=cgen_DefList(tree->children[3],indent+8);
//       l_DefList=strlen(DefList);                        
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
