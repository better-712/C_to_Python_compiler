#include <iostream>
#include <vector>
#include "type.cpp"
#include "ast.hpp"
#include <string.h>
#include "symbol.hpp"
namespace SPL {
  //INT FLOAT CHAR ARRAY  STRUCT
  Symbol_Table *cur_table=new Symbol_Table;
  
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
  
  int analyze_Specifier_FunDec_CompSt (Node* tree){
    printf("analyze_Specifier_FunDec_CompSt\n");
    return 0;
  }
  
  void defPureTypeVisit(Node *node) {
    Node *decList = node->get_nodes(1);
    string name = getStrValueFromDecList(decList);
    auto _type = snt[std::get<string>(node->get_nodes(0, 0)->value)];
    do {
        if (symbolTable.count(name) != 0) {
            variableRedefined(std::get<int>(node->value), name);
        }
        const auto &PrimitiveType = Type::getPrimitiveType(_type);
        if (decList->get_nodes(0, 0)->nodes.size() == 1) {
            symbolTable[name] = PrimitiveType;
            if (decList->get_nodes(0)->nodes.size() == 3) {
                checkTypeMatchType(symbolTable[std::get<string>(decList->get_nodes(0, 0, 0)->value)],
                                   decList->get_nodes(0, 2)->type, std::get<int>(node->value), nonMatchTypeBothSide);
            }
        } else {
            symbolTable[name] = new Type(name, CATEGORY::ARRAY,
                                         getArrayFromVarDec(decList->get_nodes(0, 0), PrimitiveType));
            if (decList->get_nodes(0)->nodes.size() == 3) {
                nonMatchTypeBothSide(std::get<int>(decList->value));
            }
        }
        if (decList->nodes.size() == 1) {
            break;
        }
        decList = decList->get_nodes(2);
        name = getStrValueFromDecList(decList);
    } while (true);
}
  
  void extDefVisit_SES_PureType(Node *tree) {
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
    Node *extDecList = tree->children[1];
    std::cout<<"extDecList: "<<extDecList->type<<std::endl;
    
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
