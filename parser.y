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

%type<Node *>Program ExtDefList

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
$$=new Node($1);
driver.set_root($$);
}
ExtDefList : ExtDef ExtDefList{$$=new Node("ExtDefList",new Node("empty"),new Node("test"));}
    | %empty{$$=new Node("empty");}
ExtDef : Specifier ExtDecList SEMI{}
    | Specifier SEMI{}
    | Specifier FunDec CompSt{}
ExtDecList : VarDec{std::cout << "ExtDecList - > (VarDec)" << std::endl;}
    | VarDec COMMA ExtDecList{std::cout << "ExtDecList - > (VarDec COMMA ExtDecList)" << std::endl;}

/* specifier */
Specifier : TYPE{std::cout << "Specifier - > (TYPE) " << $1->lexeme << std::endl;}
    | StructSpecifier{std::cout << "Specifier - > (StructSpecifier)" << std::endl;}
StructSpecifier : STRUCT ID LC DefList RC{std::cout << "StructSpecifier - > (STRUCT ID LC DefList RC)" << std::endl;}
    | STRUCT ID{std::cout << "StructSpecifier - > (STRUCT ID)" << std::endl;}

/* declarator */
VarDec : ID{std::cout << "VarDec - > (ID) " << $1->lexeme << std::endl;}
    | VarDec LB INT RB{std::cout << "VarDec - > (VarDec LB INT RB)" << std::endl;}
FunDec : ID LP VarList RP{std::cout << "FunDec - > (ID LP VarList RP) " << $1->lexeme << std::endl;}
    | ID LP RP{std::cout << "FunDec - > (ID LP RP) " << $1->lexeme << std::endl;}
VarList : ParamDec COMMA VarList{std::cout << "VarList - > (ParamDec COMMA VarList)" << std::endl;}
    | ParamDec{std::cout << "VarList - > (ParamDec)" << std::endl;}
ParamDec : Specifier VarDec{std::cout << "ParamDec - > (Specifier VarDec)" << std::endl;}

/* statement */
CompSt : LC DefList StmtList RC{std::cout << "CompSt - > (LC DefList StmtList RC)" << std::endl;}
StmtList : Stmt StmtList{std::cout << "StmtList - > (Stmt StmtList)" << std::endl;}
    | %empty{std::cout << "StmtList - > (%empty)" << std::endl;}
Stmt : Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt

/* local definition */
DefList : Def DefList
    | %empty{}
Def : Specifier DecList SEMI
DecList : Dec
    | Dec COMMA DecList
Dec : VarDec
    | VarDec ASSIGN Exp

/* Expression */
Exp : Exp ASSIGN Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp LT Exp
    | Exp LE Exp
    | Exp GT Exp
    | Exp GE Exp
    | Exp NE Exp
    | Exp EQ Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp MUL Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
    | CHAR
Args : Exp COMMA Args
    | Exp

%%

void SPL::SPL_Parser::error(const location_type &l, const std::string &err_message){
    std::cerr << "Auto parser error! " << l.begin << ": unknown " << err_message << std::endl;
    throw SPL_Parser::syntax_error(l, err_message);
}
