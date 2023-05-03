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
  char* cgen_Specifier_ExtDecList_SEMI(Node* tree, int indent){
    return(char*)"BBB";
  }

  char* cgen_ExtDef (Node* tree, int indent){
    printf("cgen_Line\n");
    
    
    char *line, *result;
    int l_line;
    
    result=(char*)"AAA";
      
    Node *tmp=tree->children[1];
    if(tmp->type.compare("ExtDecList") == 0)
      line = cgen_Specifier_ExtDecList_SEMI(tree, indent);
    else if(tmp->type.compare("SEMI") == 0)
      printf("SEMI\n");
    else if(tmp->type.compare("FunDec") == 0)
      printf("FunDec\n");
    
    l_line = strlen(line);
    result = calloc(l_line + 2, sizeof(char));
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
