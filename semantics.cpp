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
  vector<Symbol*> arg_type_global;
  
  int op[6][6] = {
    /*                 Null     int        float    char     struct    array     */
    /* Null   */    {  undef,   undef,     undef,   undef,   undef,    undef},
    /* int    */    {  undef,   _int,      _float,  undef,   undef,   undef},
    /* float  */    {  undef,   _float,    _float,  undef,   undef,   undef},
    /* char */      {  undef,   undef,     undef,   undef,   undef,   undef},
    /* struct   */  {  undef,   undef,     undef,   undef,   undef,   undef},
    /* array   */   {  undef,   undef,     undef,   undef,   undef,   undef}
    
};
  
  Symbol_Type analyze_ID (Node* tree);
  Symbol_Type analyze_Exp(Node *tree);
  void analyze_CompSt (Node* tree);
  
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
  Symbol* find_symbol(std::string name){
    Symbol_Table *level=cur_table;
    while(level!=nullptr){
      level->print_table();
      if(level->table.count(name) != 0){
        //std::cout<<"find symbol:"<<level->table[name]->name<<std::endl;
        return level->table[name];
      }
      level=level->next;
    }
    
    return nullptr;
  }
  
  void enter_scope(){
    Symbol_Table *top=new Symbol_Table;
    top->next=cur_table;
    cur_table=top;
    //printf("enter_scope\n");
  }
  void pop_scope(){
    cur_table=cur_table->next;
    //printf("pop_scope\n");
  }
  
  void record_Spec(Node* tree){
    //to do for struct
    
    if(tree->children[0]->type.compare("TYPE")==0){
      
      //cur_specifier.id=(char*)(tree->children[0]->value).c_str();
      if(tree->children[0]->value.compare("int")==0)
      {
        cur_specifier.type=INT;
        cur_specifier.value=0;
      }
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
    res.value=stoi(tree->value);
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
    Symbol* tmp=find_symbol(tree->value);
    if(tmp!= nullptr){
      return tmp->symbol_type;
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
  void analyze_Args(Node *tree) {
    printf("analyze_Args\n");
    Symbol_Type type=analyze_ID(tree->children[0]->children[0]);
    //analyze_Exp(tree->children[0]);
    if(tree->children.size()==3)
      analyze_Args(tree->children[2]);
  }
  
  Symbol_Type analyze_Exp(Node *tree) {
    if (tree->children[0]->type.compare("INT") == 0)
        return analyze_Int(tree->children[0]);
    if (tree->children[0]->type.compare("ID") == 0&&tree->children.size()==1)
        return analyze_ID(tree->children[0]);
    if (tree->children[0]->type.compare("FLOAT") == 0)
        return analyze_FLOAT(tree->children[0]);
    if (tree->children[0]->type.compare("CHAR") == 0)
        return analyze_CHAR(tree->children[0]);
    if (tree->children.size()==3&&tree->children[1]->type.compare("ASSIGN") == 0){
      Symbol_Type exp_lift=analyze_Exp(tree->children[0]);
      Symbol_Type exp_right=analyze_Exp(tree->children[2]);
      if(exp_lift.type==exp_right.type){
        exp_lift.value=exp_right.value;
      }else{
        printf("unmatch type on assignment at line:%d\n",tree->children[1]->line_no);
      }
    }
    else if (tree->children.size()==3&&tree->children[0]->type.compare("Exp") == 0&&tree->children[2]->type.compare("Exp") == 0){
      Symbol_Type res;
      Symbol_Type exp1=analyze_Exp(tree->children[0]);
      Symbol_Type exp2=analyze_Exp(tree->children[2]);
      res.type=op[exp1.type][exp2.type];
      if(res.type==1){
        //std::cout<<"exp1.value:"<<exp1.value<<std::endl;
        //std::cout<<"exp2.value:"<<exp2.value<<std::endl;
        if(tree->children[1]->type.compare("PLUS")==0)
          res.value=exp1.value+exp2.value;
        if(tree->children[1]->type.compare("MINUS")==0)
          res.value=exp1.value-exp2.value;
        if(tree->children[1]->type.compare("MUL")==0)
          res.value=exp1.value*exp2.value;
        if(tree->children[1]->type.compare("DIV")==0){
          if(exp2.value==0)//to do
             printf("divider can not be 0\n");
          res.value=exp1.value/exp2.value;
        }
        
//           std::cout<<"result:"<<exp1.value+exp2.value<<std::endl;
      }
      //std::cout<<"result:"<<res.value<<std::endl;
      //std::cout<<"res.type:"<<res.type<<std::endl;
      return res;
    }
    //LP Exp RP
    if (tree->children.size()==3&&tree->children[0]->type.compare("LP") == 0&&tree->children[2]->type.compare("RP") == 0){
       return analyze_Exp(tree->children[1]);    
    }
    //MINUS Exp
    if (tree->children.size()==2&&tree->children[0]->type.compare("MINUS") == 0){
      Symbol_Type exp=analyze_Exp(tree->children[1]);
      exp.value=-exp.value;
      return exp;
    }
    //Exp LB Exp RB |array index
    if (tree->children.size()==4&&tree->children[1]->type.compare("LB") == 0&&tree->children[3]->type.compare("RB") == 0){
      //to do a[3]
      Symbol_Type exp=analyze_Exp(tree->children[0]);
      Symbol_Type index=analyze_Exp(tree->children[2]);
      int si=exp.size;
      int in=index.value;
     // printf("indexing number:%d\n",in);
      if(si==-1)
        printf("Subscripted value is not an array at line:%d\n",tree->children[1]->line_no);
      else if(in==-10000){
        printf("indexing by non-integer\n");
      }else if(in<0||in>si-1){
        printf("indexing out of bound%d:\n",in);
      }
      return exp;
    }
    //Exp DOT ID |struct fun
    if (tree->children.size()==3&&tree->children[1]->type.compare("DOT") == 0){
      Symbol_Type exp=analyze_Exp(tree->children[0]);
      
      Symbol_Type ret;
      if(exp.type!=4){
        std::cout <<"accessing with non-struct variable at line: "<<tree->children[2]->line_no << std::endl;
      }
      for (const auto& element : exp.parm_type) {
        if(tree->children[2]->value.compare(element->name)==0)
          ret=element->symbol_type;
      }
      //std::cout <<"Dot type: "<< ret.type << std::endl;
      return ret;
    }
    //ID LP RP
    //ID LP ARGS RP
    if (tree->children.size()==4&&tree->children[0]->type.compare("ID") == 0){
      Symbol_Type id=analyze_ID(tree->children[0]);
      if(id.type==-1)return id;;
      int size=id.arg_type.size();
      //std::cout <<"size: "<< size << std::endl;
      
      //analyze_Args(tree->children[2]);
      //to do
      std::vector<Node *>* exp_list=list_to_e(tree->children[2]);
      //std::cout <<"list size: "<< exp_list->size() << std::endl;
      if(size!=exp_list->size())
        std::cout <<"function arg number unmatch,expect:"<< size<<" got "<<exp_list->size()<< std::endl;
      else{
        for(int i=0;i<exp_list->size();i++){
         Symbol_Type exp=analyze_Exp((*exp_list)[i]);
       //  std::cout <<"exp type: "<< exp.type << std::endl;
        // std::cout <<"arg type: "<< id.arg_type[i]->symbol_type.type << std::endl;
         if(exp.type!=id.arg_type[i]->symbol_type.type)
          printf("fun parm type unmatch at line:%d\n",tree->children[0]->line_no);
        }
      }
      return id;
      
      //invalid argument number for function, expect 0, got 5
    }
    
  }
  
  Symbol* analyze_VarDec(Node *tree) {
   // printf("analyze_VarDec\n");
    
    int line_no=tree->children[0]->line_no;
    Symbol* a=new Symbol;
    a->line_no=line_no;
    a->symbol_type.type=cur_specifier.type;
    a->symbol_type.value=cur_specifier.value;
    a->symbol_type.parm_type=cur_specifier.parm_type;
    a->symbol_type.tag=cur_specifier.tag;
    
    //only used in function.   to do:must follow by=
    if(tree->children.size() == 3){
      a->name=tree->children[0]->children[0]->value;
//       a->symbol_type.type=ARRAY;
      a->symbol_type.size=-1;
      //printf("size -1\n");
    }else if(tree->children.size() == 4){
      a->name=tree->children[0]->children[0]->value;
//       a->symbol_type.type=ARRAY;
      a->symbol_type.size=std::stoi(tree->children[2]->value);
    //  printf("size %d\n",a->symbol_type.size);
    }else{
      a->name=tree->children[0]->value;
    }
    
//     for (const auto& element : a->symbol_type->parm_type) {
//         std::cout <<"name: "<< element->name << " ";
//     }
//     std::cout << std::endl;
    
    
    //std::cout<<"name:"<<name<<std::endl;
    if(cur_table->table.count(a->name) != 0){
      std::cout<<"VariableRedefined:"<<a->name<<std::endl;
    }else
      cur_table->table[a->name]=a;
    return a;
  }
  
  void analyze_ExtDecList(Node *tree) {
   // printf("analyze_ExtDecList\n");
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
  void extDefVisit_SES_StructType(Node *tree) {
    
    record_Spec(tree->children[0]);
    analyze_ExtDecList(tree->children[1]);  
    //std::cout<<"cur_specifier.type: "<<cur_specifier.type<<std::endl;
    eliminate_Spec();

}

  
  int analyze_Specifier_ExtDecList_SEMI (Node* tree){
   // printf("analyze_Specifier_ExtDecList_SEMI\n");
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
  //  printf("analyze_Dec\n");
    Symbol* s=analyze_VarDec(tree->children[0]);
    Symbol_Type var=s->symbol_type;
    std::string name =s->name;
    
    if(tree->children.size() == 3){
      Symbol_Type exp=analyze_Exp(tree->children[2]);
      
      //std::cout<<"var:"<<var.type<<std::endl;
      //std::cout<<"exp:"<<exp.type<<std::endl;
      //to assign
      if(var.type==exp.type&&exp.type==1)
        cur_table->table[name]->symbol_type.value=exp.value;
      
      
      //to do 
      if(var.type!=exp.type){
        std::cout<<"type not match at line:"<<s->line_no<<std::endl;
      }
    }
    
  }
  
  void analyze_DecList (Node* tree){
  //  printf("analyze_DecList\n");
    analyze_Dec(tree->children[0]);
    if(tree->children.size() == 3){
      analyze_DecList(tree->children[2]);
    }
  }
  
  void analyze_Def (Node* tree){
   // printf("analyze_Def\n");
    //std::cout<<"tree:"<<tree->type<<std::endl;
    
    record_Spec(tree->children[0]);
    analyze_DecList(tree->children[1]);
    eliminate_Spec();
  }
  
  void analyze_DefList (Node* tree){
   // printf("analyze_DefList\n");
    //std::cout<<"tree:"<<tree->type<<std::endl;
    
    if(tree->type.compare("empty") == 0)return;
    analyze_Def(tree->children[0]);
    analyze_DefList(tree->children[1]);
    return;    
  }
  
  int analyze_StructSpecifier (Node* tree){
   // printf("analyze_StructSpecifier\n");
    int result=0;
    
    std::string name=tree->children[1]->value;
    int line_no=tree->children[1]->line_no;
    //std::cout<<"name:"<<name<<std::endl;
    
    //STRUCT ID LC DefList RC  DefList : Def DefList
    if(tree->children.size()==5){
      Symbol* a=new Symbol;
      a->name=name;
      a->symbol_type.type=STRUCT;
      a->symbol_type.tag=name;
      a->line_no=line_no;
      
      //enter scope
      enter_scope();
      
      analyze_DefList(tree->children[3]);
      int i=0;
      for (const auto& pair : cur_table->table) {
        a->symbol_type.parm_type.push_back(pair.second);
        
        //std::cout << "type name: " << a->symbol_type.parm_type[i]->name  << std::endl;
        i++;
      }
      
      pop_scope();
      
      cur_table->insert(a);
      
    }else if(tree->children.size()==3)//STRUCT ID  to do:record
    {
                           
    }
    
    return result;
  }
  
  void analyze_ParamDec (Node* tree){
    
    record_Spec(tree->children[0]);
    
    Symbol* var=analyze_VarDec(tree->children[1]);
    arg_type_global.push_back(var);
    
  }
  
  void analyze_VarList (Node* tree){
   // printf("analyze_VarList\n");
        
    analyze_ParamDec(tree->children[0]);
    
    if(tree->children.size() == 3)
      analyze_VarList(tree->children[2]);
  }
  
  void analyze_Stmt (Node* tree){
    if(tree->children[0]->type.compare("Exp") == 0){
       analyze_Exp(tree->children[0]);
       return;
     }
    
    if(tree->children[0]->type.compare("CompSt") == 0)
       return analyze_CompSt(tree->children[0]);
    
    if(tree->children[0]->type.compare("RETURN") == 0){
       //cgen_Return
      // printf("RETURN\n");
       
       Symbol_Type exp=analyze_Exp(tree->children[1]);
       
      
       return ;
     }
    
  }
  
  void analyze_StmtList (Node* tree){
   // printf("analyze_StmtList\n");
    if(tree->type.compare("empty") == 0)
      return ;
   
    analyze_Stmt(tree->children[0]);
    
    analyze_StmtList(tree->children[1]);
    
    return ;
  }
  
  void analyze_CompSt (Node* tree){
  //  printf("analyze_CompSt\n");
    
    analyze_DefList(tree->children[1]);
    
    analyze_StmtList(tree->children[2]);
    
    
  }
  
  void analyze_FunDec (Node* tree){
   // printf("analyze_FunDec\n");
    //ID LP VarList RP |ID LP RP
    std::string name=tree->children[0]->value;
    int line_no=tree->children[0]->line_no;
    //std::cout<<"name:"<<name<<std::endl;
    
    //VarList : ParamDec COMMA VarList
    Symbol* a=new Symbol;
    a->name=name;
    a->symbol_type.type=cur_specifier.type;
    a->symbol_type.parm_type=cur_specifier.parm_type;
    a->symbol_type.tag=cur_specifier.tag;
    a->line_no=line_no;
      
    enter_scope();
    if(tree->children.size()==4){
      
      analyze_VarList(tree->children[2]);
      a->symbol_type.arg_type=arg_type_global;
      arg_type_global.clear();
//       int i=0;
//       for (const auto& pair : cur_table->table) {
//         a->symbol_type.arg_type.push_back(pair.second);
        
//         std::cout << "arg_type name: " << a->symbol_type.arg_type[i]->name  << std::endl;
//         i++;
//       }
    }
    
    cur_table->next->insert(a);
    
  }
  
  int analyze_Specifier_FunDec_CompSt (Node* tree){
  //  printf("analyze_Specifier_FunDec_CompSt\n");
    record_Spec(tree->children[0]);
    
    analyze_FunDec(tree->children[1]);
   
    analyze_CompSt(tree->children[2]);
    pop_scope();
    return 0;
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
    
  //  printf("analyze_Program\n");
    return result;   
  }
  
  int semantic_analyze (Node *root){
    return analyze_Program(root);
  }
    
}
