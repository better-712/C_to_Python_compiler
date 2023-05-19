#include <iostream>
#include <vector>
#include "type.cpp"
#include "ast.hpp"
#include <string.h>
namespace SPL {
  //char* cgen_Stmt(Node* tree, int indent);
  struct spec{
    int type;
    char* id;
  };
  struct spec cur_spec={0,(char*)"un_def"};
  int INT=1;
  int FLOAT=2;
  int CHAR=3;
  int STRUCT=4;
  int ARRAY=5;
  
  int self=0;
  
  char* cgen_CompSt(Node* tree, int indent);
  
  int INDENT_LEV=4;
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
  char* cgen_FLOAT (Node* tree) {
    return (char*)(tree->value).c_str();
  }
  char* cgen_CHAR (Node* tree) {
    return (char*)(tree->value).c_str();
  }
  char* cgen_ID(Node* tree){
    return (char*)(tree->value).c_str();
  }
  
  char* cgen_op(Node* tree){
//     if(tree->children[0]->type.compare("ASSIGN") == 0)
//       return (char*)"=";
//     if(tree->children[0]->type.compare("AND") == 0)
//       return (char*)"&&";
//     if(tree->children[0]->type.compare("OR") == 0)
//       return (char*)"||";
//     if(tree->children[0]->type.compare("LT") == 0)
//       return (char*)"<";
//     if(tree->children[0]->type.compare("LE") == 0)
//       return (char*)"<=";
//     if(tree->children[0]->type.compare("GT") == 0)
//       return (char*)">";
//     if(tree->children[0]->type.compare("GE") == 0)
//       return (char*)">=";
//     if(tree->children[0]->type.compare("NE") == 0)
//       return (char*)"!=";
//     if(tree->children[0]->type.compare("EQ") == 0)
//       return (char*)"==";
//     if(tree->children[0]->type.compare("PLUS") == 0)
//       return (char*)"+";
//     if(tree->children[0]->type.compare("MINUS") == 0)
//       return (char*)"-";
//     if(tree->children[0]->type.compare("MUL") == 0)
//       return (char*)"*";
//     if(tree->children[0]->type.compare("DIV") == 0)
//       return (char*)"/";
    return (char*)(tree->value).c_str();
  }
  
  void record_Specifier(Node* tree){
    //to do for struct
    
    if(tree->children[0]->type.compare("TYPE")==0){
      
      cur_spec.id=(char*)(tree->children[0]->value).c_str();
      if(tree->children[0]->value.compare("int")==0)
        cur_spec.type=INT;
      if(tree->children[0]->value.compare("float")==0)
        cur_spec.type=FLOAT;
      if(tree->children[0]->value.compare("char")==0)
        cur_spec.type=CHAR;
     // std::cout<<cur_spec.id<<std::endl;
    }else{
      //StructSpecifier
      //STRUCT ID
      
      cur_spec.type=STRUCT;
      cur_spec.id=cgen_ID(tree->children[0]->children[1]);
     // std::cout<<"cur_spec.id:"<<cur_spec.id<<"cur_spec.type:"<<cur_spec.type<<std::endl;
    }
    
  }
  
  void eliminate_Specifier(){
    cur_spec.type=0;
    cur_spec.id=(char*)"un_def";
  }
 
  
  char* cgen_Exp(Node* tree){
    if (tree->children[0]->type.compare("INT") == 0)
        return cgen_Int(tree->children[0]);
    if (tree->children[0]->type.compare("ID") == 0)
        return cgen_ID(tree->children[0]);
    if (tree->children[0]->type.compare("FLOAT") == 0)
        return cgen_FLOAT(tree->children[0]);
    if (tree->children[0]->type.compare("CHAR") == 0)
        return cgen_CHAR(tree->children[0]);
    if (tree->children.size()==3&&tree->children[0]->type.compare("Exp") == 0&&tree->children[2]->type.compare("Exp") == 0){
      char  *exp1,*op,*exp2,*result;
      int l_exp1,l_op,l_exp2;
      exp1=cgen_Exp(tree->children[0]);
      l_exp1=strlen(exp1);
      op=cgen_op(tree->children[1]);
      l_op=strlen(op);
      exp2=cgen_Exp(tree->children[2]);
      l_exp2=strlen(exp2);
      result = (char*)calloc(l_exp1 + l_op + l_exp2, sizeof(char));
      memcpy(result, exp1, l_exp1 * sizeof(char));
      memcpy(result+l_exp1, op, l_op * sizeof(char));
      memcpy(result+l_exp1+l_op, exp2, l_exp2 * sizeof(char));
      return result;
    }
    //LP Exp RP
    if (tree->children.size()==3&&tree->children[0]->type.compare("LP") == 0&&tree->children[2]->type.compare("RP") == 0){
      char  *exp,*result;
      int l_exp;
      exp=cgen_Exp(tree->children[1]);
      l_exp=strlen(exp);
      result = (char*)calloc(l_exp + 2 , sizeof(char));
      result[0]='(';
      memcpy(result+1, exp, l_exp * sizeof(char));
      result[l_exp+1]=')';
      return result;
    }
    //MINUS Exp
    if (tree->children.size()==2&&tree->children[0]->type.compare("MINUS") == 0){
      char  *exp,*result;
      int l_exp;
      exp=cgen_Exp(tree->children[1]);
      l_exp=strlen(exp);
      result = (char*)calloc(l_exp + 1 , sizeof(char));
      result[0]='-';
      memcpy(result+1, exp, l_exp * sizeof(char));
      return result;
    }
    //Exp LB Exp RB
    if (tree->children.size()==4&&tree->children[1]->type.compare("LB") == 0&&tree->children[3]->type.compare("RB") == 0){
      char  *exp1,*exp2,*result;
      int l_exp1,l_exp2;
      exp1=cgen_Exp(tree->children[0]);
      l_exp1=strlen(exp1);
      exp2=cgen_Exp(tree->children[2]);
      l_exp2=strlen(exp2);
      result = (char*)calloc(l_exp1 + 2 + l_exp2, sizeof(char));
      memcpy(result, exp1, l_exp1 * sizeof(char));
      result[l_exp1]='[';
      memcpy(result+l_exp1+1, exp2, l_exp2 * sizeof(char));
      result[l_exp1+1+l_exp2]=']';
      return result;
    }
    //Exp DOT ID
    if (tree->children.size()==3&&tree->children[1]->type.compare("DOT") == 0){
      char  *exp,*id,*result;
      int l_exp,l_id;
      exp=cgen_Exp(tree->children[0]);
      l_exp=strlen(exp);
      id=cgen_ID(tree->children[2]);
      l_id=strlen(id);
      result = (char*)calloc(l_exp + 2 + l_id, sizeof(char));
      memcpy(result, exp, l_exp * sizeof(char));
      result[l_exp]='.';
      memcpy(result+l_exp+1, id, l_id * sizeof(char));
      return result;
    }
    
    return (char*)"cgen_Exp";
  }
  
  
  char* cgen_VarDec(Node* tree){
    char  *result;
    if(tree->children.size() == 3)
    {
      char *id;
      int l_id;
      id = cgen_VarDec(tree->children[0]);
      l_id= strlen(id);
      result = (char*)calloc(l_id + 1, sizeof(char));
      memcpy(result, id, l_id * sizeof(char));
      
    }else if(tree->children.size() == 4)
    {
      char *id,*in;
      int l_id,l_in;
      id = cgen_VarDec(tree->children[0]);
      l_id= strlen(id);
      in=cgen_Int(tree->children[2]);
      l_in= strlen(in);
      result = (char*)calloc(l_id + 1 , sizeof(char));
      memcpy(result, id, l_id * sizeof(char));
      //result[l_id] = '[';
      //memcpy(result+l_id , in, l_in * sizeof(char));
      //result[l_id + 1 + l_in] = ']';
      //arr
      cur_spec.type=ARRAY;
      //value
      cur_spec.id=in;
      
    }else{
      result=cgen_ID(tree->children[0]);
    }
    return result;
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
    
    int len=strlen(cur_spec.id);
    
    if (l_ext > 0)
      result = (char*)calloc(l_var + 7 + l_ext+len, sizeof(char));
    else
      result = (char*)calloc(l_var+7+len, sizeof(char));
    
    memcpy(result, var, l_var * sizeof(char));
    //struct to do
    if(cur_spec.type==STRUCT){
      result[l_var]='=';
      memcpy(result+l_var+1, cur_spec.id, len * sizeof(char));
      memcpy(result+l_var+1+len, "()\n", 3 * sizeof(char));
    }else if(cur_spec.type==ARRAY){
      //[0]*9
      memcpy(result+l_var,"=[0]*", 5 * sizeof(char));
      memcpy(result+l_var+5, cur_spec.id, len * sizeof(char));
      result[l_var+5+len]='\n';
    }else if(cur_spec.type==INT){
      //[0]*9
      memcpy(result+l_var,"=0\n", 3 * sizeof(char));
      
    }else if(cur_spec.type==FLOAT){
      //[0]*9
      memcpy(result+l_var,"=0.0\n", 5 * sizeof(char));
      
    }else
      memcpy(result+l_var, "=None\n", 6 * sizeof(char));
    
    
    if (l_ext > 0){  
        memcpy(result + l_var + 6, extDecList, l_ext * sizeof(char));
    }
    
    return result;
  }
  char* cgen_Specifier_ExtDecList_SEMI(Node* tree, int indent){
    char  *ExtDecList, *result;
    int  l_ext;

    ExtDecList = result = NULL;
    l_ext = 0;
    
    //Specifier = cgen_Specifier(tree->children[0]);
    //l_spec = strlen(Specifier);
    record_Specifier(tree->children[0]);
    
    ExtDecList = cgen_ExtDecList(tree->children[1]);
    l_ext = strlen(ExtDecList);
    
    result = (char*)calloc(l_ext, sizeof(char));
    
    memcpy(result, ExtDecList, l_ext * sizeof(char));
        
    eliminate_Specifier();
    return result;
  }
  
  char* cgen_ParamDec(Node* tree){
    char * VarDec, *result;
    int l_VarDec;
    VarDec = result = NULL;
    l_VarDec = 0;
//     Specifier = cgen_Specifier(tree->children[0]);
//     l_spec = strlen(Specifier);
    VarDec = cgen_VarDec(tree->children[1]);
    l_VarDec = strlen(VarDec);
    
    result = (char*)calloc(l_VarDec, sizeof(char));
//     memcpy(result, Specifier, l_spec * sizeof(char));
//     result[l_spec] = ' ';
    memcpy(result, VarDec, l_VarDec * sizeof(char));
        
    return result;
  }
  
  char* cgen_VarList(Node* tree){
    char *ParamDec,*VarList, *result;
    int l_ParamDec,l_VarList;
    ParamDec = VarList = result = NULL;
    l_ParamDec = l_VarList = 0;
    
    ParamDec = cgen_ParamDec(tree->children[0]);
    l_ParamDec = strlen(ParamDec);
    if(tree->children.size() == 3){
      VarList= cgen_VarList(tree->children[2]);
      l_VarList = strlen(VarList);
    }
    
    if (l_VarList > 0)
      result = (char*)calloc(l_ParamDec + 1 + l_VarList, sizeof(char));
    else
      result = (char*)calloc(l_ParamDec, sizeof(char));
    
    memcpy(result, ParamDec, l_ParamDec * sizeof(char));
    if (l_VarList > 0){
        result[l_ParamDec] = ',';
        memcpy(result + l_ParamDec + 1, VarList, l_VarList * sizeof(char));
    }
    
    return result;
  }
  
  char* cgen_FunDec(Node* tree){
    char *id,*VarList,*result;
    int l_id,l_VarList;
    id=cgen_ID(tree->children[0]);
    l_id=strlen(id);
    if(tree->children.size() == 4){
      VarList=cgen_VarList(tree->children[2]);
      l_VarList=strlen(VarList);
    }
    //ID LP RP
    if(tree->children.size() == 3)
      l_VarList=0;
    
    result = (char*)calloc(l_id + 4 +l_VarList, sizeof(char));
    memcpy(result, id, l_id * sizeof(char));
    result[l_id] = '(';
    if(l_VarList!=0)memcpy(result+l_id+1, VarList, l_VarList * sizeof(char));
    result[l_id+l_VarList+1] = ')';
    result[l_id+l_VarList+2] = ':';
    return result;
  }
  //result = (char*)calloc(indent+1 +l_DecList, sizeof(char));
  //  memset(result, ' ', indent * sizeof(char));
  
  char* cgen_Dec(Node* tree, int indent){
    //indent
    char *result;
    result=NULL;
    //struct or None
    if(tree->children.size()==1){
      char *var;
      int l_var;
      var=NULL;
      l_var=0;
      
      var = cgen_VarDec(tree->children[0]);
      l_var = strlen(var);
      
      int se=0;
      if(self==1)se=5;
      
      int len=strlen(cur_spec.id);
      
      result = (char*)calloc(indent+l_var+7+se+len, sizeof(char));
      memset(result, ' ', indent * sizeof(char));
      memcpy(result+indent, "self.", se * sizeof(char));
      memcpy(result+indent+se, var, l_var * sizeof(char));
      
      
      if(cur_spec.type==STRUCT){
        result[indent+se+l_var]='=';
        memcpy(result+indent+se+l_var+1, cur_spec.id, len * sizeof(char));
        memcpy(result+indent+se+l_var+1+len, "()\n", 3 * sizeof(char));
        
      }else if(cur_spec.type==ARRAY){
      //[0]*9
      memcpy(result+indent+se+l_var,"=[0]*", 5 * sizeof(char));
      memcpy(result+indent+se+l_var+5, cur_spec.id, len * sizeof(char));
      }else if(cur_spec.type==INT){
      
      memcpy(result+indent+se+l_var,"=0\n", 3 * sizeof(char));
      
      }else if(cur_spec.type==FLOAT){
      //[0]*9
      memcpy(result+indent+se+l_var,"=0.0\n", 5 * sizeof(char));
      
      }else
        memcpy(result+indent+l_var+se, "=None\n", 6 * sizeof(char)); 
      
    }
    else{
      char *var,*exp;
      int l_var,l_exp;
      var=exp=NULL;
      l_var=l_exp=0;
      var = cgen_VarDec(tree->children[0]);
      l_var = strlen(var);
      exp = cgen_Exp(tree->children[2]);
      l_exp = strlen(exp);
      
      int se=0;
      if(self==1)se=5;
      result = (char*)calloc(indent+l_var+l_exp+2+se, sizeof(char));
      memset(result, ' ', indent * sizeof(char));
      memcpy(result+indent, "self.", se * sizeof(char));
      memcpy(result+indent+se, var, l_var * sizeof(char));
      result[indent+l_var+se]='=';
      memcpy(result+indent+l_var+1+se, exp, l_exp * sizeof(char));
      
    }
    return result;
  }
  
  char* cgen_DecList(Node* tree, int indent){
    char *Dec,*DecList, *result;
    int l_Dec,l_DecList;
    Dec = DecList = result = NULL;
    l_Dec = l_DecList = 0;
    
    Dec = cgen_Dec(tree->children[0],indent);
    l_Dec = strlen(Dec);
    if(tree->children.size() == 3){
      DecList= cgen_DecList(tree->children[2],indent);
      l_DecList = strlen(DecList);
    }
    
    if (l_DecList > 0)
      result = (char*)calloc(l_Dec + 1 + l_DecList, sizeof(char));
    else
      result = (char*)calloc(l_Dec, sizeof(char));
    
    memcpy(result, Dec, l_Dec * sizeof(char));
    if (l_DecList > 0){
        result[l_Dec] = '\n';
        memcpy(result + l_Dec + 1, DecList, l_DecList * sizeof(char));
    }
    
    return result;
  }
  
  char* cgen_Def(Node* tree, int indent){
    //indent
    char  *DecList, *result;
    int  l_DecList;

    DecList = result = NULL;
    l_DecList = 0;
    
//     Specifier = cgen_Specifier(tree->children[0]);
//     l_spec = strlen(Specifier);
    record_Specifier(tree->children[0]);
    
    
    DecList = cgen_DecList(tree->children[1],indent);
    l_DecList = strlen(DecList);
    
    
    result = (char*)calloc(1 +l_DecList, sizeof(char));
    //memset(result, ' ', indent * sizeof(char));
//     memcpy(result+indent, Specifier, l_spec * sizeof(char));
//     result[indent+l_spec] = ' ';
    memcpy(result, DecList, l_DecList * sizeof(char));
       
    eliminate_Specifier();
    return result;
  }
  
  char* cgen_DefList(Node* tree, int indent){
    //null? ""!
    if(tree->type.compare("empty") == 0)return (char*)"";
    char *Def,*DefList, *result;
    int l_Def,l_DefList;
    Def = DefList = result = NULL;
    l_Def = l_DefList = 0;
    
    Def = cgen_Def(tree->children[0],indent);
    l_Def = strlen(Def);
    DefList= cgen_DefList(tree->children[1],indent);
    l_DefList = strlen(DefList);
    
    result = (char*)calloc(l_Def + l_DefList+2, sizeof(char));
    memcpy(result, Def, l_Def * sizeof(char));
    result[l_Def]='\n';
    memcpy(result+l_Def+1, DefList, l_DefList * sizeof(char));
    return result;
  }
  
  char* cgen_Stmt(Node* tree, int indent,int first_if=1){
    if(tree->children[0]->type.compare("Exp") == 0){
       char * result;
       char * exp=cgen_Exp(tree->children[0]);
       int l_exp=strlen(exp);
       result = (char*)calloc(indent+l_exp+2, sizeof(char));
       memset(result, ' ', indent * sizeof(char));
       memcpy(result+indent, exp, l_exp * sizeof(char));
       result[indent+l_exp]='\n';
       return result;
     }
    if(tree->children[0]->type.compare("CompSt") == 0)
       return cgen_CompSt(tree->children[0],indent);
    
    if(tree->children[0]->type.compare("RETURN") == 0){
       //cgen_Return
       char *Exp, *result;
       int l_Exp;

       Exp=cgen_Exp(tree->children[1]);
       l_Exp=strlen(Exp);
       result = (char*)calloc(indent+9+l_Exp, sizeof(char));
       memset(result, ' ', indent * sizeof(char));
       memcpy(result + indent, "return ", 7 * sizeof(char));
       memcpy(result+indent+7, Exp, l_Exp * sizeof(char));
       result[indent+7+l_Exp]='\n';
       return result;
     }
     if(tree->children[0]->type.compare("WHILE") == 0){
       char *Exp,*Stmt, *result;
       int l_Exp,l_Stmt;
       Exp=cgen_Exp(tree->children[2]);
       l_Exp=strlen(Exp);
       Stmt=cgen_Stmt(tree->children[4],indent+INDENT_LEV);
       l_Stmt=strlen(Stmt);
       result = (char*)calloc(indent+5+l_Exp+3+l_Stmt+1, sizeof(char));
       memset(result, ' ', indent * sizeof(char));
       memcpy(result + indent, "while ", 6 * sizeof(char));
       memcpy(result+indent+6, Exp, l_Exp * sizeof(char));
       memcpy(result + indent+6+l_Exp, ":", 1 * sizeof(char));
       memcpy(result + indent+7+l_Exp, Stmt, l_Stmt * sizeof(char));
       return result;
     }
    //IF LP Exp RP Stmt if score > 60:
     if(tree->children.size()==5&&tree->children[0]->type.compare("IF") == 0){
       char *Exp,*Stmt, *result;
       int l_Exp,l_Stmt;
       
       Exp=cgen_Exp(tree->children[2]);
       l_Exp=strlen(Exp);
       Stmt=cgen_Stmt(tree->children[4],indent+INDENT_LEV);
       l_Stmt=strlen(Stmt);
       
       result = (char*)calloc(indent+6+l_Exp+l_Stmt, sizeof(char));
       memset(result, ' ', indent * sizeof(char));
       memcpy(result + indent, "if ", 3 * sizeof(char));
       memcpy(result+indent+3, Exp, l_Exp * sizeof(char));
       memcpy(result + indent+3+l_Exp, ":\n", 2 * sizeof(char));
       memcpy(result + indent+5+l_Exp, Stmt, l_Stmt * sizeof(char));
       return result;
     }
    //if(exp)else stmt el  if stmt
     if(tree->children.size()==7&&tree->children[0]->type.compare("IF") == 0&&tree->children[6]->children[0]->type.compare("IF") == 0){
       
       char *Exp,*Stmt1,*Stmt2, *result;
       int l_Exp,l_Stmt1,l_Stmt2;
       
       Exp=cgen_Exp(tree->children[2]);
       l_Exp=strlen(Exp);
       Stmt1=cgen_Stmt(tree->children[4],indent+INDENT_LEV);
       l_Stmt1=strlen(Stmt1);
       Stmt2=cgen_Stmt(tree->children[6],indent,0);
       l_Stmt2=strlen(Stmt2);
       
       if(first_if){
         result = (char*)calloc(2*indent+9+l_Exp+l_Stmt1+l_Stmt2, sizeof(char));
         memset(result, ' ', indent * sizeof(char));
         memcpy(result + indent, "if ", 3 * sizeof(char));
         memcpy(result+indent+3, Exp, l_Exp * sizeof(char));
         memcpy(result + indent+3+l_Exp, ":\n", 2 * sizeof(char));
         memcpy(result + indent+5+l_Exp, Stmt1, l_Stmt1 * sizeof(char));
         memset(result+ indent+5+l_Exp+l_Stmt1, ' ', indent * sizeof(char));
         memcpy(result+ 2*indent+5+l_Exp+l_Stmt1, "el", 2 * sizeof(char));
         memcpy(result+ 2*indent+7+l_Exp+l_Stmt1, Stmt2, l_Stmt2 * sizeof(char));
       
       }else{
         result = (char*)calloc(indent+9+l_Exp+l_Stmt1+l_Stmt2, sizeof(char));
         memcpy(result, "if ", 3 * sizeof(char));
         memcpy(result+3, Exp, l_Exp * sizeof(char));
         memcpy(result+3+l_Exp, ":\n", 2 * sizeof(char));
         memcpy(result+5+l_Exp, Stmt1, l_Stmt1 * sizeof(char));
         memset(result+5+l_Exp+l_Stmt1, ' ', indent * sizeof(char));
         memcpy(result+indent+5+l_Exp+l_Stmt1, "el", 2 * sizeof(char));
         memcpy(result+indent+7+l_Exp+l_Stmt1, Stmt2, l_Stmt2 * sizeof(char));
       
       }
       
       
       return result;
     }
    ////IF LP Exp RP Stmt ELSE Stmt
     if(tree->children.size()==7&&tree->children[0]->type.compare("IF") == 0){
       
       char *Exp,*Stmt1,*Stmt2, *result;
       int l_Exp,l_Stmt1,l_Stmt2;
       
       Exp=cgen_Exp(tree->children[2]);
       l_Exp=strlen(Exp);
       Stmt1=cgen_Stmt(tree->children[4],indent+INDENT_LEV);
       l_Stmt1=strlen(Stmt1);
       Stmt2=cgen_Stmt(tree->children[6],indent+INDENT_LEV);
       l_Stmt2=strlen(Stmt2);
       
       
       if(first_if){
       result = (char*)calloc(2*indent+11+l_Exp+l_Stmt1+l_Stmt2, sizeof(char));
       memset(result, ' ', indent * sizeof(char));
       memcpy(result + indent, "if ", 3 * sizeof(char));
       memcpy(result+indent+3, Exp, l_Exp * sizeof(char));
       memcpy(result + indent+3+l_Exp, ":\n", 2 * sizeof(char));
       memcpy(result + indent+5+l_Exp, Stmt1, l_Stmt1 * sizeof(char));
       memset(result+ indent+5+l_Exp+l_Stmt1, ' ', indent * sizeof(char));
       memcpy(result+ 2*indent+5+l_Exp+l_Stmt1, "else:\n", 6 * sizeof(char));
       memcpy(result+ 2*indent+11+l_Exp+l_Stmt1, Stmt2, l_Stmt2 * sizeof(char));
       
       }else{
         result = (char*)calloc(indent+11+l_Exp+l_Stmt1+l_Stmt2, sizeof(char));
         memcpy(result, "if ", 3 * sizeof(char));
         memcpy(result+3, Exp, l_Exp * sizeof(char));
         memcpy(result+3+l_Exp, ":\n", 2 * sizeof(char));
         memcpy(result+5+l_Exp, Stmt1, l_Stmt1 * sizeof(char));
         memset(result+5+l_Exp+l_Stmt1, ' ', indent * sizeof(char));
         memcpy(result+ indent+5+l_Exp+l_Stmt1, "else:\n", 6 * sizeof(char));
         memcpy(result+ indent+11+l_Exp+l_Stmt1, Stmt2, l_Stmt2 * sizeof(char));
       }
       return result;
     }
    //FOR Exp Stmt
     if(tree->children.size()==3&&tree->children[0]->type.compare("FOR") == 0){
       char *Exp,*Stmt, *result;
       int l_Exp,l_Stmt;
       
       Exp=cgen_ID(tree->children[1]->children[2]->children[0]);
       l_Exp=strlen(Exp);
       
       
       Stmt=cgen_Stmt(tree->children[2],indent+INDENT_LEV);
       l_Stmt=strlen(Stmt);
       
       result = (char*)calloc(indent+20+l_Exp+l_Stmt, sizeof(char));
       memset(result, ' ', indent * sizeof(char));
       memcpy(result + indent, "for i in range(", 15 * sizeof(char));
       memcpy(result+indent+15, Exp, l_Exp * sizeof(char));
       memcpy(result + indent+15+l_Exp, "):\n", 3 * sizeof(char));
       memcpy(result + indent+18+l_Exp, Stmt, l_Stmt * sizeof(char));
       return result;
     }
    
    //Exp SEMI
    //CompSt
    //RETURN Exp SEMI
    //IF LP Exp RP Stmt
    //IF LP Exp RP Stmt ELSE Stmt
    //WHILE LP Exp RP Stmt
    
    return (char*)"cgen_Stmt";
  }
  
  char* cgen_StmtList(Node* tree, int indent){
    //Stmt StmtList
    if(tree->type.compare("empty") == 0)return (char*)"";
    char *Stmt,*StmtList, *result;
    int l_Stmt,l_StmtList;
    Stmt = StmtList = result = NULL;
    l_Stmt = l_StmtList = 0;
    
    Stmt = cgen_Stmt(tree->children[0],indent);
    l_Stmt = strlen(Stmt);
    StmtList= cgen_StmtList(tree->children[1],indent);
    l_StmtList = strlen(StmtList);
    
    result = (char*)calloc(l_Stmt + l_StmtList+1, sizeof(char));
    memcpy(result, Stmt, l_Stmt * sizeof(char));
    //result[l_Stmt]='\n';
    memcpy(result+l_Stmt, StmtList, l_StmtList * sizeof(char));
    return result;
  }
  
  char* cgen_CompSt(Node* tree, int indent){
    char *DefList,*StmtList,*result;
    int l_DefList,l_StmtList;
    DefList=cgen_DefList(tree->children[1],indent);
    l_DefList=strlen(DefList);
    StmtList=cgen_StmtList(tree->children[2],indent);
    l_StmtList=strlen(StmtList);
    
    if(tree->type.compare("SEMI") == 0||l_DefList+l_StmtList==0){
      result = (char*)calloc(indent+7, sizeof(char));
      result[0] = '\n';
      memset(result+1, ' ', indent * sizeof(char));
      memcpy(result+1+indent, "pass\n", 5 * sizeof(char));
      return result;
    }
    
    result = (char*)calloc(l_DefList + 5 +l_StmtList, sizeof(char));
    
    result[0] = '\n';
    memcpy(result+1, DefList, l_DefList * sizeof(char));
    memcpy(result+1+l_DefList, StmtList, l_StmtList * sizeof(char));
    
    return result;
  }
  
  char* cgen_Specifier_FunDec_CompSt(Node* tree, int indent){
    char *Specifier, *FunDec,*CompSt, *result;
    int l_spec, l_fun, l_comp;

    Specifier = FunDec =CompSt= result = NULL;
    l_spec = l_fun =l_comp= 0;
    
    Specifier = (char*)"def";
    l_spec = strlen(Specifier);
    
    FunDec = cgen_FunDec(tree->children[1]);
    l_fun = strlen(FunDec);
    
    CompSt = cgen_CompSt(tree->children[2],indent+INDENT_LEV);
    l_comp = strlen(CompSt);
    
    result = (char*)calloc(l_spec + l_fun +l_comp+2, sizeof(char));
    memcpy(result, Specifier, l_spec * sizeof(char));
    result[l_spec] = ' ';
    memcpy(result+l_spec+1, FunDec, l_fun * sizeof(char));
    memcpy(result+l_spec+1+l_fun, CompSt, l_comp * sizeof(char));
    return result;
  }
  char* cgen_StructSpecifier (Node* tree, int indent){
    
    //STRUCT ID
    char* id,*result;
    result=(char*)"cgen_StructSpecifier";
    int l_id=0;
    id=cgen_ID(tree->children[1]);
    l_id=strlen(id);
    
    //DefList : Def DefList
    self=1;
    if(tree->children.size()==5&&tree->children[3]->type.compare("empty")==0){
      result = (char*)calloc(18+l_id+indent, sizeof(char));
      memcpy(result, "class ", 6 * sizeof(char));
      memcpy(result+6, id, l_id * sizeof(char));
      memcpy(result+6+l_id, ":\n", 2 * sizeof(char));
      memset(result+8+l_id, ' ', (indent+4) * sizeof(char));
      memcpy(result+12+l_id+indent, "pass\n", 5 * sizeof(char));
      
    }else if(tree->children.size()==5){
      //def __init__(self):
      char * DefList;
      int l_DefList;
      DefList=cgen_DefList(tree->children[3],indent+8);
      l_DefList=strlen(DefList);                  
                           
      result = (char*)calloc(l_id+l_DefList+indent+32, sizeof(char));
      memcpy(result, "class ", 6 * sizeof(char));
      memcpy(result+6, id, l_id * sizeof(char));
      memcpy(result+6+l_id, ":\n", 2 * sizeof(char));
      
      memset(result+8+l_id, ' ', (indent+4) * sizeof(char));
      memcpy(result+l_id+indent+12, "def __init__(self):\n", 20 * sizeof(char));
      memcpy(result+l_id+indent+32, DefList, l_DefList * sizeof(char));
      
    }
    //STRUCT ID LC DefList RC
    self=0;
    return result;
  }

  char* cgen_ExtDef (Node* tree, int indent){
    //printf("cgen_Line\n");
    
    
    char *line, *result;
    int l_line;
      
    Node *tmp=tree->children[1];
    if(tmp->type.compare("ExtDecList") == 0)
      line = cgen_Specifier_ExtDecList_SEMI(tree, indent);
    else if(tmp->type.compare("SEMI") == 0)
      line = cgen_StructSpecifier(tree->children[0]->children[0], indent);
    else if(tmp->type.compare("FunDec") == 0)
      line = cgen_Specifier_FunDec_CompSt(tree, indent);
    
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
        //printf("line is %s\n", lines[nLines]);
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
    
    printf("Python code: \n%s\n", result);
    return result;
    
  }
  
  char* code_gen (Node *root){
    //printf("code_gen\n");
    return cgen_Program(root, 0);
  }
  
  
}
