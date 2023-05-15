#include <iostream>
#include <vector>
#include "type.cpp"
#include "ast.hpp"
#include <string.h>
namespace SPL {
  //INT FLOAT CHAR ARRAY  STRUCT
  
  
  int analyze_Specifier_FunDec_CompSt (Node* tree){
    printf("analyze_Specifier_FunDec_CompSt\n");
    return 0;
  }
  
  int analyze_Specifier_ExtDecList_SEMI (Node* tree){
    printf("analyze_Specifier_ExtDecList_SEMI\n");
    return 0;
  }
  int analyze_StructSpecifier (Node* tree){
    printf("analyze_StructSpecifier\n");
    int result=0;
    
    
    int l_id=0;
    id=cgen_ID(tree->children[1]);
    l_id=strlen(id);
    string name=tree->children[1]->value;
    std::cout<<"name:"<<name<<std::endl;
    
    //STRUCT ID LC DefList RC  DefList : Def DefList
    if(tree->children.size()==5){
      
      
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
    std::vector<Node *>* list=list_to_ele(ExtDefList);
    
    nLines=list->size();
    
    for(auto iter=list->begin();iter!=list->end();iter++){
        result = result+cgen_ExtDef((*iter), indent);
        
        nLines++;
    }
    
    printf("analyze_Program\n");
    return result;   
  }
  
  int semantic_analyze (Node *root){
    return cgen_Program(root);
  }
    
}
