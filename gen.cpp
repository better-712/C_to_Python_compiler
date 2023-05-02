#include <iostream>
#include <vector>
#include "type.cpp"

namespace SPL {
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

  char* cgen_Line (Node* tree, int indent){
    printf("cgen_Line\n");
    return "abc";
  }
  char* cgen_Program (Node* tree, int indent){
    char *result;
    int nLines, total, last, l_line;
    Node *ExtDefList=tree->children[0];

    result = NULL;
    nLines = last = total = l_line = 0;
    std::vector<Node *>* list=list_to_element(ExtDefList)
    
    nLines=list.size();
    char* lines[nLines];
    nLines = 0;
    for(auto iter=list->begin();iter!=list->end();iter++){
        lines[nLines] = cgen_Line(tmp, indent);
        printf("line is %s\n", lines[nLines]);
        if (lines[nLines] == NULL)
            printf("lines[%d] == NULL\n",nLines);
        total += strlen(lines[nLines]);
        nLines++;
    }
    result = calloc(total + 1, sizeof(char));
    
    for (int i=0; i<nLines; i++) {
        l_line = strlen(lines[i]);
        memcpy(result + last, lines[i], l_line * sizeof(char));
        last += l_line;
        free(lines[i]);
    }
   
    
  }
  
  char* code_gen (Node *root){
    printf("code_gen\n");
    return cgen_Program(root, 0);
  }
  
  
}
