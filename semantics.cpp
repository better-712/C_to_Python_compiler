#include <iostream>
#include <vector>
#include "type.cpp"
#include "ast.hpp"
#include <string.h>
#include "symbol.hpp"
namespace SPL {
  //INT FLOAT CHAR ARRAY  STRUCT
  #define _int 1
  #define _float 2
  #define undef -1
  
  extern int INT,FLOAT,CHAR,STRUCT,ARRAY;
  Symbol_Table *cur_table=new Symbol_Table;
  Symbol_Type cur_specifier;
  
  int op[6][6] = {
    /*                 Null     int        float    char     struct    array     */
    /* Null   */    {  undef,   undef,     undef,   undef,   undef,    undef},
    /* int    */    {  undef,   _int,      _float,  undef,   undef,   undef},
    /* float  */    {  undef,   _float,    _float,  undef,   undef,   undef},
    /* char */      {  undef,   undef,     undef,   undef,   undef,   undef},
    /* struct   */  {  undef,   undef,     undef,   undef,   undef,   undef},
    /* array   */   {  undef,   undef,     undef,   undef,   undef,   undef}
    
};
  
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
      cur_specifier=analyze_ID(tree->children[0]->children[1]);
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
  
  Symbol_Type analyze_ID (Node* tree) {
    Symbol_Type res;
    //to do all table
    if(cur_table->table.count(tree->value) != 0){
      return cur_table->table[tree->value]->symbol_type;
    }
    else{
      //to do
      if(cur_specifier.type==STRUCT&&cur_specifier.tag.compare("tag")==0)
        std::cout<<"STRUCTUndefined:"<<tree->value<<std::endl;
      else
        std::cout<<"VariableUndefined:"<<tree->value<<std::endl;
    }
    return res;
  }
  
  int analyze_Specifier_FunDec_CompSt (Node* tree){
    printf("analyze_Specifier_FunDec_CompSt\n");
    
    return 0;
  }
  
  Symbol_Type analyze_Exp(Node *tree) {
    if (tree->children[0]->type.compare("INT") == 0)
        return analyze_Int(tree->children[0]);
    if (tree->children[0]->type.compare("ID") == 0)
        return analyze_ID(tree->children[0]);
    if (tree->children[0]->type.compare("FLOAT") == 0)
        return analyze_FLOAT(tree->children[0]);
    if (tree->children[0]->type.compare("CHAR") == 0)
        return analyze_CHAR(tree->children[0]);
    if (tree->children.size()==3&&tree->children[0]->type.compare("Exp") == 0&&tree->children[2]->type.compare("Exp") == 0){
      Symbol_Type res;
      Symbol_Type exp1=analyze_Exp(tree->children[0]);
      Symbol_Type exp2=analyze_Exp(tree->children[2]);
      res.type=op[exp1.type][exp2.type];
      std::cout<<"res.type:"<<res.type<<std::endl;
      return res;
    }
    //LP Exp RP
    if (tree->children.size()==3&&tree->children[0]->type.compare("LP") == 0&&tree->children[2]->type.compare("RP") == 0){
       return analyze_Exp(tree->children[1]);    
    }
    //MINUS Exp
    if (tree->children.size()==2&&tree->children[0]->type.compare("MINUS") == 0){
      return analyze_Exp(tree->children[1]);
    }
    //Exp LB Exp RB array index
    if (tree->children.size()==4&&tree->children[1]->type.compare("LB") == 0&&tree->children[3]->type.compare("RB") == 0){
      
    }
    //Exp DOT ID struct fun
    if (tree->children.size()==3&&tree->children[1]->type.compare("DOT") == 0){
      
    }
    
  }
  
  Symbol_Type analyze_VarDec(Node *tree) {
    printf("analyze_VarDec\n");
    
    int line_no=tree->children[0]->line_no;
    Symbol* a=new Symbol;
    a->line_no=line_no;
    a->symbol_type.type=cur_specifier.type;
    
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
    a->symbol_type.tag=a->name;
   
    //std::cout<<"name:"<<name<<std::endl;
    if(cur_table->table.count(a->name) != 0){
      std::cout<<"VariableRedefined:"<<a->name<<std::endl;
    }else
      cur_table->table[a->name]=a;
    return a->symbol_type;
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
    
    record_Spec(tree->children[0]);
    analyze_ExtDecList(tree->children[1]);  
    //std::cout<<"cur_specifier.type: "<<cur_specifier.type<<std::endl;
    eliminate_Spec();

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
    Symbol_Type var=analyze_VarDec(tree->children[0]);
    Symbol_Type exp=analyze_Exp(tree->children[2]);
    
    std::cout<<"var:"<<var.type<<std::endl;
    std::cout<<"exp:"<<exp.type<<std::endl;
    //to do 
    if(var.type!=exp.type){
      std::cout<<"type not match:"<<a->name<<std::endl;
    }
    
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
      int i=0;
      for (const auto& pair : cur_table->table) {
        a->symbol_type.parm_type.push_back(pair.second->symbol_type);
        
        std::cout << "type tag: " << a->symbol_type.parm_type[i].tag  << std::endl;
        i++;
      }
      
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
