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

    /* include for all driver functions */
    #include "spl_driver.hpp"

    /* include for all AST functions */
    

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
list:INT{std::cout<<"hhhhhhhhhh"<<std::endl;}
    |FLOAT
    |CHAR
    |TYPE
    |ID
    |STRUCT
    |IF
    |ELSE
    |WHILE
    |RETURN
    |ASSIGN
    |DOT
    |SEMI
    |COMMA
    |LP
    |RP
    |LC
    |RC
    |LB
    |RB
    |AND
    |OR
%%

void SPL::SPL_Parser::error(const location_type &l, const std::string &err_message){
    std::cerr << "Auto parser error! " << l.begin << ": unknown " << err_message << std::endl;
    throw SPL_Parser::syntax_error(l, err_message);
}
