%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {SPL}
%define parser_class_name {SPL_Parser}

%code requires{
   namespace SPL {
      class Scan_Info;

      class SPL_Driver;
      class SPL_Scanner;
      class Node;
      
   }

}

%parse-param { SPL_Scanner  &scanner  }
%parse-param { SPL_Driver  &driver  }

%code{
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    #include <string>

    #define LOCAL1

    #include "spl_driver.hpp"
    #include "ast.hpp"

    #undef yylex
    #define yylex scanner.yylex

}

%define api.value.type variant
%define parse.assert
%language "C++"
%locations

%token               END    0     "end of file"
%token <Scan_Info *> INT
%token <Scan_Info *> FLOAT
%token <Scan_Info *> CHAR
%token <Scan_Info *> TYPE ID
%token <Scan_Info *> STRUCT IF ELSE WHILE RETURN
%token <Scan_Info *> ASSIGN
%token <Scan_Info *> DOT SEMI COMMA
%token <Scan_Info *> EQ LE LT GE GT NE
%token <Scan_Info *> PLUS MINUS MUL DIV
%token <Scan_Info *> AND OR NOT
%token <Scan_Info *> LP RP LC RC LB RB
%token <Scan_Info *> LINE_COMMENT
%token <Scan_Info *> ERROR

%type<Node *>Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args

%nonassoc ERROR
%right ASSIGN
%left OR
%left AND
%left LT LE GT GE EQ NE
%left PLUS SUB
%left MUL DIV
%right NOT
%left DOT
%left LB RB
%left LP RP

%start Program

%%

/* High-level definition */
Program : ExtDefList{
$$=new Node("Program",$1);
driver.set_root($$);
}
ExtDefList : ExtDef ExtDefList{$$=new Node("ExtDefList",$1,$2);}
    | %empty{$$=new Node("empty");}
ExtDef : Specifier ExtDecList SEMI{$$=new Node("ExtDef",$1,$2,new Node("SEMI",$3));}
    | Specifier SEMI{$$=new Node("ExtDef",$1,new Node("SEMI",$2));}
    | Specifier FunDec CompSt{$$=new Node("ExtDef",$1,$2,$3);}
ExtDecList : VarDec{$$ = new Node("ExtDecList",$1);}
    | VarDec COMMA ExtDecList{$$ = new Node("ExtDecList",$1,new Node("COMMA",$2),$3);}

/* specifier */
Specifier : TYPE{$$ = new Node("Specifier",new Node("TYPE", $1));}
    | StructSpecifier{$$ = new Node("Specifier",$1);}
StructSpecifier : STRUCT ID LC DefList RC{$$ = new Node("StructSpecifier",new Node("STRUCT",$1),new Node("ID",$2),new Node("LC",$3),$4,new Node("RC",$5));}
    | STRUCT ID{$$ = new Node("StructSpecifier",new Node("STRUCT",$1),new Node("ID",$2));}

/* declarator */
VarDec : ID{$$ = new Node("VarDec",new Node("ID",$1));}
    | VarDec LB INT RB{$$ = new Node("VarDec",$1,new Node("LB",$2),new Node("INT",$3),new Node("RB",$4));}
FunDec : ID LP VarList RP{$$ = new Node("FunDec",new Node("ID",$1),new Node("LP",$2),$3,new Node("RP",$4));}
    | ID LP RP{$$ = new Node("FunDec",new Node("ID",$1),new Node("LP",$2),new Node("RP",$3));}
VarList : ParamDec COMMA VarList{$$ = new Node("VarList",$1,new Node("COMMA",$2),$3);}
    | ParamDec{$$ = new Node("VarList",$1);}
ParamDec : Specifier VarDec{$$ = new Node("ParamDec",$1,$2);}

/* statement */
CompSt : LC DefList StmtList RC{$$ = new Node("CompSt",new Node("LC",$1),$2,$3,new Node("RC",$4));}
StmtList : Stmt StmtList{$$ = new Node("StmtList",$1,$2);}
    | %empty{$$=new Node("empty");}
Stmt : Exp SEMI{$$ = new Node("Stmt",$1,new Node("SEMI",$2));}
    | CompSt{$$ = new Node("Stmt",$1);}
    | RETURN Exp SEMI{$$ = new Node("Stmt",new Node("RETURN",$1),$2,new Node("SEMI",$3));}
    | IF LP Exp RP Stmt{$$ = new Node("Stmt",new Node("IF",$1),new Node("LP",$2),$3,new Node("RP",$4),$5);}
    | IF LP Exp RP Stmt ELSE Stmt{$$ = new Node("Stmt",new Node("IF",$1),new Node("LP",$2),$3,new Node("RP",$4),$5,new Node("ELSE",$6),$7);}
    | WHILE LP Exp RP Stmt{$$ = new Node("Stmt",new Node("WHILE",$1),new Node("LP",$2),$3,new Node("RP",$4),$5);}

/* local definition */
DefList : Def DefList{$$ = new Node("DefList",$1,$2);}
    | %empty{$$ = new Node("empty");}
Def : Specifier DecList SEMI{$$ = new Node("Def",$1,$2,new Node("SEMI",$3));}
DecList : Dec{$$ = new Node("DecList",$1);}
    | Dec COMMA DecList{$$ = new Node("DecList",$1,new Node("COMMA",$2),$3);}
Dec : VarDec{$$ = new Node("Dec",$1);}
    | VarDec ASSIGN Exp{$$ = new Node("Dec",$1,new Node("ASSIGN",$2),$3);}

/* Expression */
Exp : Exp ASSIGN Exp{$$ = new Node("Exp",$1,new Node("ASSIGN",$2),$3);}
    | Exp AND Exp{$$ = new Node("Exp",$1,new Node("AND",$2),$3);}
    | Exp OR Exp{$$ = new Node("Exp",$1,new Node("OR",$2),$3);}
    | Exp LT Exp{$$ = new Node("Exp",$1,new Node("LT",$2),$3);}
    | Exp LE Exp{$$ = new Node("Exp",$1,new Node("LE",$2),$3);}
    | Exp GT Exp{$$ = new Node("Exp",$1,new Node("GT",$2),$3);}
    | Exp GE Exp{$$ = new Node("Exp",$1,new Node("GE",$2),$3);}
    | Exp NE Exp{$$ = new Node("Exp",$1,new Node("NE",$2),$3);}
    | Exp EQ Exp{$$ = new Node("Exp",$1,new Node("EQ",$2),$3);}
    | Exp PLUS Exp{$$ = new Node("Exp",$1,new Node("PLUS",$2),$3);}
    | Exp MINUS Exp{$$ = new Node("Exp",$1,new Node("MINUS",$2),$3);}
    | Exp MUL Exp{$$ = new Node("Exp",$1,new Node("MUL",$2),$3);}
    | Exp DIV Exp{$$ = new Node("Exp",$1,new Node("DIV",$2),$3);}
    | LP Exp RP{$$ = new Node("Exp",new Node("LP",$1),$2,new Node("RP",$3));}
    | MINUS Exp{$$ = new Node("Exp",new Node("MINUS",$1),$2);}
    | NOT Exp{$$ = new Node("Exp",new Node("NOT",$1),$2);}
    | ID LP Args RP{$$ = new Node("Exp",new Node("ID",$1),new Node("LP",$2),$3,new Node("RP",$4));}
    | ID LP RP{$$ = new Node("Exp",new Node("ID",$1),new Node("LP",$2),new Node("RP",$3));}
    | Exp LB Exp RB{$$ = new Node("Exp",$1,new Node("LB",$2),$3,new Node("RB",$4));}
    | Exp DOT ID{$$ = new Node("Exp",$1,new Node("DOT",$2),new Node("ID",$3));}
    | ID{$$ = new Node("Exp",new Node("ID",$1));}
    | INT{$$ = new Node("Exp",new Node("INT",$1));}
    | FLOAT{$$ = new Node("Exp",new Node("FLOAT",$1));}
    | CHAR{$$ = new Node("Exp",new Node("CHAR",$1));}
Args : Exp COMMA Args{"Args",$$ = new Node($1,new Node("COMMA",$2),$3);}
    | Exp{$$ = new Node("Args",$1);}

%%

void SPL::SPL_Parser::error(const location_type &l, const std::string &err_message){
    std::cerr << "Auto parser error! " << l.begin << ": unknown " << err_message << std::endl;
    throw SPL_Parser::syntax_error(l, err_message);
}
