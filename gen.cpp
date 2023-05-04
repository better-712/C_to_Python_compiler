#include <iostream>
#include <vector>
#include "type.cpp"
#include "ast.hpp"
#include <string.h>
namespace SPL {
  std::vector<Node *>* list_to_ele(Node *node){
        auto *decs = new std::vector<Node *>{};
        decs->push_back(node->children.front());
        Node *list=node->children.back();
        while(list->type.compare(node->type) == 0){
            decs->push_back(list->children.front());
            list=list->children.back();
        }
        return decs;
    }
  char* cgen_Int (Node* tree) {
    
    return (char*)(tree->value).c_str();
}
  
  char* cgen_VarDec(Node* tree){
    char  *result;
    if(tree->children.size() == 4)
    {
      char *id,*in;
      int l_id,l_in;
      id = cgen_VarDec(tree->children[0]);
      l_id= strlen(id);
      in=cgen_Int(tree->children[2]);
      l_in= strlen(in);
      result = (char*)calloc(l_id + 2 + l_in, sizeof(char));
      memcpy(result, id, l_id * sizeof(char));
      result[l_id] = '[';
      memcpy(result+l_id + 1, in, l_in * sizeof(char));
      result[l_id + 1 + l_in] = ']';
    }else{
      result=(char*)(tree->children[0]->value).c_str();
    }
    return result;
  }
  char* cgen_Specifier(Node* tree){
    return (char*)"A+";
  }
  char* cgen_ExtDecList(Node* tree){
    char *var,*extDecList, *result;
    int l_var,l_ext;
    var = extDecList = result = NULL;
    l_var = l_ext = 0;
    
    var = cgen_VarDec(tree->children[0]);
    l_var = strlen(var);
    if(tree->children.size() == 3){
      extDecList= cgen_ExtDecList(tree->children[2]);
      l_ext = strlen(extDecList);
    }
    
    if (l_ext > 0)
      result = (char*)calloc(l_var + 1 + l_ext, sizeof(char));
    else
      result = (char*)calloc(l_var, sizeof(char));
    
    memcpy(result, var, l_var * sizeof(char));
    if (l_ext > 0){
        result[l_var] = ',';
        memcpy(result + l_var + 1, extDecList, l_ext * sizeof(char));
    }
    
    return result;
  }
  char* cgen_Specifier_ExtDecList_SEMI(Node* tree, int indent){
    char *Specifier, *ExtDecList, *result;
    int l_spec, l_ext;

    Specifier = ExtDecList = result = NULL;
    l_spec = l_ext = 0;
    
    Specifier = cgen_Specifier(tree->children[0]);
    l_spec = strlen(Specifier);
    
    ExtDecList = cgen_ExtDecList(tree->children[1]);
    l_ext = strlen(ExtDecList);
    
    result = (char*)calloc(l_spec + 1 +l_ext, sizeof(char));
    memcpy(result, Specifier, l_spec * sizeof(char));
    result[l_spec] = ' ';
    memcpy(result + l_spec + 1, ExtDecList, l_ext * sizeof(char));
        
    return result;
  }

  char* cgen_ExtDef (Node* tree, int indent){
    printf("cgen_Line\n");
    
    
    char *line, *result;
    int l_line;
      
    Node *tmp=tree->children[1];
    if(tmp->type.compare("ExtDecList") == 0)
      line = cgen_Specifier_ExtDecList_SEMI(tree, indent);
    else if(tmp->type.compare("SEMI") == 0)
      printf("SEMI\n");
    else if(tmp->type.compare("FunDec") == 0)
      printf("FunDec\n");
    
    l_line = strlen(line);
    result = (char*)calloc(l_line + 2, sizeof(char));
    memcpy(result, line, l_line * sizeof(char));
    // Add the ENDLINE TOKEN
    memset(result + l_line, '\n', sizeof(char));
    
    return result;
  }
  char* cgen_Program (Node* tree, int indent){
    char *result;
    int nLines, total, last, l_line;
    Node *ExtDefList=tree->children[0];

    result = NULL;
    nLines = last = total = l_line = 0;
    std::vector<Node *>* list=list_to_ele(ExtDefList);
    
    nLines=list->size();
    char* lines[nLines];
    nLines = 0;
    for(auto iter=list->begin();iter!=list->end();iter++){
        lines[nLines] = cgen_ExtDef((*iter), indent);
        printf("line is %s\n", lines[nLines]);
        if (lines[nLines] == NULL)
            printf("lines[%d] == NULL\n",nLines);
        total += strlen(lines[nLines]);
        nLines++;
    }
    result = (char*)calloc(total + 1, sizeof(char));
    
    for (int i=0; i<nLines; i++) {
        l_line = strlen(lines[i]);
        memcpy(result + last, lines[i], l_line * sizeof(char));
        last += l_line;
        //free(lines[i]);
    }
   
    
  }
  
  char* code_gen (Node *root){
    printf("code_gen\n");
    return cgen_Program(root, 0);
  }
  
  
}
