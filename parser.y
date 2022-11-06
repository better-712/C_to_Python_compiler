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
      class AST_Node;
      class Leaf_Node;
      class Program_Node;
      class ExtDefList_Node;
      class ExtDef_Node;
      class ExtDecList_Node;
      class Specifier_Node;
      class StructSpecifier_Node;
      class VarDec_Node;
      class FunDec_Node;
      class VarList_Node;
      class ParamDec_Node;
      class CompSt_Node;
      class StmtList_Node;
      class Stmt_Node;
      class DefList_Node;
      class Def_Node;
      class DecList_Node;
      class Dec_Node;
      class Exp_Node;
      class Args_Node;
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

%type <Program_Node *> Program
%type <ExtDefList_Node *> ExtDefList
%type <ExtDef_Node *> ExtDef
%type <ExtDecList_Node *> ExtDecList
%type <Specifier_Node *> Specifier
%type <StructSpecifier_Node *> StructSpecifier
%type <VarDec_Node *> VarDec
%type <FunDec_Node *> FunDec
%type <VarList_Node *> VarList
%type <ParamDec_Node *> ParamDec
%type <CompSt_Node *> CompSt
%type <StmtList_Node *> StmtList
%type <Stmt_Node *> Stmt
%type <DefList_Node *> DefList
%type <Def_Node *> Def
%type <DecList_Node *> DecList
%type <Dec_Node *> Dec
%type <Exp_Node *> Exp
%type <Args_Node *> Args

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
Program
  : ExtDefList {
  	#ifdef LOCAL
  	    std::cout << "Program -> (ExtDefList)" << std::endl;
  	#endif
  	$$ = new Program_Node($1);
  	driver.set_root($$);
  }
  ;

ExtDefList
  : ExtDef ExtDefList {
  	#ifdef LOCAL
  	    std::cout << "ExtDefList - > (ExtDef ExtDefList)" << std::endl;
  	#endif
  	$$ = new ExtDefList_Node($1, $2);
  }
  | %empty {
  	#ifdef LOCAL
  	    std::cout << "ExtDefList - > (%empty)" << std::endl;
  	#endif
  	$$ = new Empty_ExtDefList_Node();
  }
  ;

ExtDef
  : Specifier ExtDecList SEMI {
    	#ifdef LOCAL
  	    std::cout << "ExtDef - > (Specifier ExtDecList SEMI)" << std::endl;
  	#endif
  	$$ = new ExtDef_Node($1, $2, make_leaf(token::SEMI, $3));
  }
  | Specifier ExtDecList %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "ExtDef - > (Specifier ExtDecList %prec ERROR)" << std::endl;
  	#endif
  	driver.add_syntax_error(";", $1);
  	$$ = new ExtDef_Node($1, $2, make_leaf(token::SEMI, ";", $2->propagate_line_no()));
  }
  | Specifier SEMI {
  	#ifdef LOCAL
  	    std::cout << "ExtDef - > (Specifier SEMI)" << std::endl;
  	#endif
  	$$ = new ExtDef_Node($1, make_leaf(token::SEMI, $2));

  }
  | Specifier %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "ExtDef - > (Specifier %prec ERROR)" << std::endl;
  	#endif
  	driver.add_syntax_error(";", $1);
  	$$ = new ExtDef_Node($1, make_leaf(token::SEMI, ";", $1->propagate_line_no()));
  }
  | Specifier FunDec CompSt {
  	#ifdef LOCAL
  	    std::cout << "ExtDef - > (Specifier FunDec CompSt)" << std::endl;
  	#endif
  	$$ = new ExtDef_Node($1, $2, $3);
  }
  ;

ExtDecList
  : VarDec {
  	#ifdef LOCAL
  	    std::cout << "ExtDecList - > (VarDec)" << std::endl;
  	#endif
  	$$ = new ExtDecList_Node($1);
  }
  | VarDec COMMA ExtDecList {
  	#ifdef LOCAL
  	    std::cout << "ExtDecList - > (VarDec COMMA ExtDecList)" << std::endl;
  	#endif
  	$$ = new ExtDecList_Node($1, make_leaf(token::COMMA, $2), $3);
  }
  | VarDec ExtDecList %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "ExtDef - > (VarDec ExtDecList %prec ERROR)" << std::endl;
  	#endif
  	driver.add_syntax_error(";", $1);
  	$$ = new ExtDecList_Node($1, make_leaf(token::COMMA, ";", $2->propagate_line_no()), $2);
  }
  ;


/* specifier */
Specifier
  : TYPE {
  	#ifdef LOCAL
  	    std::cout << "Specifier - > (TYPE) " << $1->lexeme << std::endl;
  	#endif
  	$$ = new Specifier_Node(make_leaf(token::TYPE, $1));
  }
  | StructSpecifier {
  	#ifdef LOCAL
  	    std::cout << "Specifier - > (StructSpecifier)" << std::endl;
  	#endif
  	$$ = new Specifier_Node($1);
  }
  ;

StructSpecifier
  : STRUCT ID LC DefList RC {
  	#ifdef LOCAL
  	    std::cout << "StructSpecifier - > (STRUCT ID LC DefList RC)" << std::endl;
  	#endif
  	$$ = new StructSpecifier_Node(make_leaf(token::STRUCT, $1),
  				      make_leaf(token::ID, $2),
  				      make_leaf(token::LC, $3),
  				      $4,
  				      make_leaf(token::RC, $5));
  }
  | STRUCT ID LC DefList %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "StructSpecifier - > (STRUCT ID LC DefList %prec ERROR)" << std::endl;
  	#endif
  	driver.add_syntax_error("}", $1);
  	$$ = new StructSpecifier_Node(make_leaf(token::STRUCT, $1),
  				      make_leaf(token::ID, $2),
  				      make_leaf(token::LC, $3),
  				      $4,
  				      make_leaf(token::RC, "}", $4->propagate_line_no()));
  }
  | STRUCT ID {
  	#ifdef LOCAL
  	    std::cout << "StructSpecifier - > (STRUCT ID)" << std::endl;
  	#endif
  	$$ = new StructSpecifier_Node(make_leaf(token::STRUCT, $1),
          			      make_leaf(token::ID, $2));
  }
  ;


/* declarator */
VarDec
  : ID {
  	#ifdef LOCAL
  	    std::cout << "VarDec - > (ID) " << $1->lexeme << std::endl;
  	#endif
	$$ = new ID_VarDec_Node(make_leaf(token::ID, $1));
  }
  | VarDec LB INT RB {
  	#ifdef LOCAL
  	    std::cout << "VarDec - > (VarDec LB INT RB)" << std::endl;
  	#endif
  	$$ = new Array_VarDec_Node($1,
			     	   make_leaf(token::LB, $2),
			     	   make_leaf(token::INT, $3),
			     	   make_leaf(token::RB, $4));
  }
  | VarDec LB INT %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "VarDec - > (VarDec LB INT %prec ERROR)" << std::endl;
  	#endif
  	driver.add_syntax_error("]", $1);
	$$ = new Array_VarDec_Node($1,
			     	   make_leaf(token::LB, $2),
			     	   make_leaf(token::INT, $3),
			     	   make_leaf(token::RB, "]", $3->line_no));
  }
  ;

FunDec
  : ID LP VarList RP {
  	#ifdef LOCAL
  	    std::cout << "FunDec - > (ID LP VarList RP) " << $1->lexeme << std::endl;
  	#endif
  	$$ = new FunDec_Node(make_leaf(token::ID, $1),
			     make_leaf(token::LP, $2),
			     $3,
			     make_leaf(token::RP, $4));
  }
  | ID LP VarList %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "FunDec - > (ID LP VarList %prec ERROR) " << $1->lexeme << std::endl;
  	#endif
    	driver.add_syntax_error(")", $1);
  	$$ = new FunDec_Node(make_leaf(token::ID, $1),
			     make_leaf(token::LP, $2),
			     $3,
			     make_leaf(token::RP, ")", $3->propagate_line_no()));
  }
  | ID LP RP {
  	#ifdef LOCAL
  	    std::cout << "FunDec - > (ID LP RP) " << $1->lexeme << std::endl;
  	#endif
  	$$ = new FunDec_Node(make_leaf(token::ID, $1),
			     make_leaf(token::LP, $2),
			     make_leaf(token::RP, $3));
  }
  | ID LP %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "FunDec - > (ID LP %prec ERROR) " << $1->lexeme << std::endl;
  	#endif
    	driver.add_syntax_error(")", $1);
  	$$ = new FunDec_Node(make_leaf(token::ID, $1),
			     make_leaf(token::LP, $2),
			     make_leaf(token::RP, ")", $2->line_no));
  }
  ;

VarList
  : ParamDec COMMA VarList {
  	#ifdef LOCAL
  	    std::cout << "VarList - > (ParamDec COMMA VarList)" << std::endl;
  	#endif
  	$$ = new VarList_Node($1,
			      make_leaf(token::COMMA, $2),
			      $3);
  }
  | ParamDec {
  	#ifdef LOCAL
  	    std::cout << "VarList - > (ParamDec)" << std::endl;
  	#endif
  	$$ = new VarList_Node($1);
  }
  ;

ParamDec
  : Specifier VarDec {
  	#ifdef LOCAL
  	    std::cout << "ParamDec - > (Specifier VarDec)" << std::endl;
  	#endif
  	$$ = new ParamDec_Node($1, $2);
  }
  ;


/* statement */
CompSt
  : LC DefList StmtList RC {
  	#ifdef LOCAL
  	    std::cout << "CompSt - > (LC DefList StmtList RC)" << std::endl;
  	#endif
  	$$ = new CompSt_Node(make_leaf(token::LC, $1),
		             $2,
		             $3,
		             make_leaf(token::RC, $4));
  }
  | LC DefList StmtList %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "CompSt - > (LC DefList StmtList %prec ERROR)" << std::endl;
  	#endif
  	driver.add_syntax_error("}", {$1});
  	$$ = new CompSt_Node(make_leaf(token::LC, $1),
		             $2,
		             $3,
		             make_leaf(token::RC, "}", $3->propagate_line_no()));
  }
  ;

StmtList
  : Stmt StmtList {
  	#ifdef LOCAL
  	    std::cout << "StmtList - > (Stmt StmtList)" << std::endl;
  	#endif
  	$$ = new StmtList_Node($1, $2);
  }
  | %empty {
  	#ifdef LOCAL
  	    std::cout << "StmtList - > (%empty)" << std::endl;
  	#endif
  	$$ = new Empty_StmtList_Node();
  }
  ;

Stmt
  : Exp SEMI {
  	#ifdef LOCAL
  	    std::cout << "Stmt - > (Exp SEMI)" << std::endl;
  	#endif
  	$$ = new Exp_Stmt_Node($1,
  			       make_leaf(token::SEMI, $2));
  }
//  | Exp %prec ERROR {
//  	driver.add_syntax_error(";", $1);
//  }
  | Exp {
  	#ifdef LOCAL
  	    std::cout << "Stmt - > (Exp)" << std::endl;
  	#endif
  	driver.add_syntax_error(";", $1);
  }
  | CompSt {
  	#ifdef LOCAL
  	    std::cout << "Stmt - > (CompSt)" << std::endl;
  	#endif
  	$$ = new CompSt_Stmt_Node($1);
  }
  | RETURN Exp SEMI {
  	#ifdef LOCAL
  	    std::cout << "Stmt - > (RETURN Exp SEMI)" << std::endl;
  	#endif
  	$$ = new Return_Stmt_Node(make_leaf(token::RETURN, $1),
  			   	  $2,
          		   	  make_leaf(token::SEMI, $3));
  }
  | RETURN Exp %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "Stmt - > (RETURN Exp %prec ERROR)" << std::endl;
  	#endif
  	driver.add_syntax_error(";", $1);
  	$$ = new Return_Stmt_Node(make_leaf(token::RETURN, $1),
  			   	  $2,
          		   	  make_leaf(token::SEMI, ";", $2->propagate_line_no()));
  }
  | IF LP Exp RP Stmt {
  	#ifdef LOCAL
  	    std::cout << "Stmt - > (IF LP Exp RP Stmt)" << std::endl;
  	#endif
  	$$ = new If_Stmt_Node(make_leaf(token::IF, $1),
  			      make_leaf(token::LP, $2),
  			      $3,
  			      make_leaf(token::RP, $4),
          		      $5);
  }
  | IF LP Exp Stmt %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "Stmt - > (IF LP Exp Stmt %prec ERROR)" << std::endl;
  	#endif
	driver.add_syntax_error(")", $1);
  	$$ = new If_Stmt_Node(make_leaf(token::IF, $1),
  			      make_leaf(token::LP, $2),
  			      $3,
  			      make_leaf(token::RP, ")", $3->propagate_line_no()),
          		      $4);
  }
  | IF LP Exp RP Stmt ELSE Stmt {
  	#ifdef LOCAL
  	    std::cout << "Stmt - > (IF LP Exp RP Stmt ELSE Stmt)" << std::endl;
  	#endif
  	$$ = new If_Stmt_Node(make_leaf(token::IF, $1),
  			      make_leaf(token::LP, $2),
  			      $3,
  			      make_leaf(token::RP, $4),
          		      $5,
          		      make_leaf(token::ELSE, $6),
          		      $7);
  }
  | WHILE LP Exp Stmt ELSE Stmt %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "Stmt - > (WHILE LP Exp Stmt ELSE Stmt %prec ERROR)" << std::endl;
  	#endif
	driver.add_syntax_error(")", $1);
  	$$ = new If_Stmt_Node(make_leaf(token::IF, $1),
  			      make_leaf(token::LP, $2),
  			      $3,
  			      make_leaf(token::RP, ")", $3->propagate_line_no()),
          		      $4,
          		      make_leaf(token::ELSE, $5),
          		      $6);
  }
  | WHILE LP Exp RP Stmt {
  	#ifdef LOCAL
  	    std::cout << "Stmt - > (WHILE LP Exp RP Stmt)" << std::endl;
  	#endif
  	$$ = new While_Stmt_Node(make_leaf(token::WHILE, $1),
			         make_leaf(token::LP, $2),
			         $3,
			         make_leaf(token::RP, $4),
			         $5);
  }
  | WHILE LP Exp Stmt %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "Stmt - > (WHILE LP Exp Stmt %prec ERROR)" << std::endl;
  	#endif
	driver.add_syntax_error(")", $1);
  	$$ = new While_Stmt_Node(make_leaf(token::WHILE, $1),
			         make_leaf(token::LP, $2),
			         $3,
			         make_leaf(token::RP, ")", $3->propagate_line_no()),
			         $4);
  }
  ;


/* local definition */
DefList
  : Def DefList {
  	#ifdef LOCAL
  	    std::cout << "DefList - > (Def DefList)" << std::endl;
  	#endif
  	$$ = new DefList_Node($1, $2);
  }
  | %empty {
  	#ifdef LOCAL
  	    std::cout << "DefList - > (%empty)" << std::endl;
  	#endif
  	$$ = new Empty_DefList_Node();
  }
  ;

Def
  : Specifier DecList SEMI {
  	#ifdef LOCAL
  	    std::cout << "Def - > (Specifier DecList SEMI)" << std::endl;
  	#endif
  	$$ = new Def_Node($1,
  	 		  $2,
  	 		  make_leaf(token::SEMI, $3));
  }
  | Specifier DecList %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "Def - > (Specifier DecList %prec ERROR)" << std::endl;
  	#endif
  	driver.add_syntax_error(";", $1);
  	$$ = new Def_Node($1,
  	 		  $2,
  	 		  make_leaf(token::SEMI, ";", $2->propagate_line_no()));
  }
  ;

DecList
  : Dec {
  	#ifdef LOCAL
  	    std::cout << "DecList - > (Dec)" << std::endl;
  	#endif
  	$$ = new DecList_Node($1);
  }
  | Dec COMMA DecList {
  	#ifdef LOCAL
  	    std::cout << "DecList - > (Dec COMMA DecList)" << std::endl;
  	#endif
  	$$ = new DecList_Node($1,
  			      make_leaf(token::COMMA, $2),
  			      $3);
  }
  ;

Dec
  : VarDec {
  	#ifdef LOCAL
  	    std::cout << "Dec - > (VarDec)" << std::endl;
  	#endif
  	$$ = new Dec_Node($1);
  }
  | VarDec ASSIGN Exp {
  	#ifdef LOCAL
  	    std::cout << "Dec - > (VarDec ASSIGN Exp)" << std::endl;
  	#endif
  	$$ = new Dec_Node($1,
  			  make_leaf(token::ASSIGN, $2),
  			  $3);
  }
  ;


/* Expression */
Exp
  : Exp ASSIGN Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp ASSIGN Exp)" << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::ASSIGN, $2), $3);
  }
  | Exp AND Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp AND Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::AND, $2), $3);
  }
  | Exp OR Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp OR Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::OR, $2), $3);
  }
  | Exp ERROR Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp ERROR Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::ERROR, $2), $3);
  }
  | Exp LT Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp LT Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::LT, $2), $3);
  }
  | Exp LE Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp LE Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::LE, $2), $3);
  }
  | Exp GT Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp GT Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::GT, $2), $3);
   }
  | Exp GE Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp GE Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::GE, $2), $3);
  }
  | Exp NE Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp NE Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::NE, $2), $3);
  }
  | Exp EQ Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (ID) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::EQ, $2), $3);
  }
  | Exp PLUS Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp PLUS Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::PLUS, $2), $3);
  }
  | Exp MINUS Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp MINUS Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::MINUS, $2), $3);
  }
  | Exp MUL Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp MUL Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::MUL, $2), $3);
  }
  | Exp DIV Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp DIV Exp) " << std::endl;
  	#endif
  	$$ = new Binary_Exp_Node($1, make_leaf(token::DIV, $2), $3);
  }
  | LP Exp RP {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (LP Exp RP) " << std::endl;
  	#endif
  	$$ = new Parentheses_Exp_Node(make_leaf(token::LP, $1),
  				      $2,
  				      make_leaf(token::RP, $3));
  }
  | LP Exp %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (LP Exp %prec ERROR) " << std::endl;
  	#endif
	driver.add_syntax_error(")", $1);
  	$$ = new Parentheses_Exp_Node(make_leaf(token::LP, $1),
  				      $2,
  				      make_leaf(token::RP, ")", $2->propagate_line_no()));
  }
  | MINUS Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (MINUS Exp) " << std::endl;
  	#endif
  	$$ = new Unary_Exp_Node(make_leaf(token::MINUS, $1), $2);
  }
  | NOT Exp {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (NOT Exp) " << std::endl;
  	#endif
  	$$ = new Unary_Exp_Node(make_leaf(token::NOT, $1), $2);
  }
  | ID LP Args RP {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (ID LP Args RP) " << $1->lexeme << std::endl;
  	#endif
	$$ = new ID_Parentheses_Exp_Node(make_leaf(token::ID, $1),
				         make_leaf(token::LP, $2),
				         $3,
				         make_leaf(token::RP, $4));
  }
  | ID LP Args %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (ID LP Args %prec ERROR) " << $1->lexeme << std::endl;
  	#endif
	driver.add_syntax_error(")", $1);
	$$ = new ID_Parentheses_Exp_Node(make_leaf(token::ID, $1),
				         make_leaf(token::LP, $2),
				         $3,
				         make_leaf(token::RP, ")", $3->propagate_line_no()));
  }
  | ID LP RP {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (ID LP RP) " << $1->lexeme << std::endl;
  	#endif
	$$ = new ID_Parentheses_Exp_Node(make_leaf(token::ID, $1),
					 make_leaf(token::LP, $2),
					 make_leaf(token::RP, $3));
  }
  | ID LP %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (ID LP %prec ERROR) " << $1->lexeme << std::endl;
  	#endif
	driver.add_syntax_error(")", $1);
	$$ = new ID_Parentheses_Exp_Node(make_leaf(token::ID, $1),
					 make_leaf(token::LP, $2),
					 make_leaf(token::RP, ")", $2->line_no));
  }
  | Exp LB Exp RB {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp LB Exp RB) " << std::endl;
  	#endif
	$$ = new Bracket_Exp_Node($1,
				  make_leaf(token::LB, $2),
				  $3,
				  make_leaf(token::RB, $4));
  }
  | Exp LB Exp %prec ERROR {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp LB Exp %prec ERROR) " << std::endl;
  	#endif
	driver.add_syntax_error("]", $1);
	$$ = new Bracket_Exp_Node($1,
				  make_leaf(token::LB, $2),
				  $3,
				  make_leaf(token::RB, "]", $3->propagate_line_no()));
  }
  | Exp DOT ID {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (Exp DOT ID) " << $3->lexeme << std::endl;
  	#endif
	$$ = new Dot_Exp_Node($1,
			      make_leaf(token::DOT, $2),
			      make_leaf(token::ID, $3));
  }
  | ID {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (ID) " << $1->lexeme << std::endl;
  	#endif
  	$$ = new Leaf_Exp_Node(make_leaf(token::ID, $1));
  }
  | INT {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (INT) " << $1->lexeme << std::endl;
  	#endif
  	$$ = new Leaf_Exp_Node(make_leaf(token::INT, $1));
  }
  | FLOAT {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (FLOAT) " << $1->lexeme << std::endl;
  	#endif
  	$$ = new Leaf_Exp_Node(make_leaf(token::FLOAT, $1));
  }
  | CHAR {
  	#ifdef LOCAL
  	    std::cout << "Exp - > (CHAR) " << $1->lexeme << std::endl;
  	#endif
  	$$ = new Leaf_Exp_Node(make_leaf(token::CHAR, $1));
  }
  | ERROR {
	#ifdef LOCAL
  	    std::cout << "Exp - > (ERROR) " << $1->lexeme << std::endl;
  	#endif
  	$$ = new Leaf_Exp_Node(make_leaf(token::ERROR, $1));
  }
  ;

Args
  : Exp COMMA Args {
  	#ifdef LOCAL
  	    std::cout << "Args - > (Exp COMMA Args) " << std::endl;
  	#endif
  	$$ = new Args_Node($1, make_leaf(token::COMMA, $2), $3);
  }
  | Exp {
        #ifdef LOCAL
  	    std::cout << "Args - > (Exp) " << std::endl;
  	#endif
        $$ = new Args_Node($1);
  }
  ;

%%

void SPL::SPL_Parser::error(const location_type &l, const std::string &err_message){
    std::cerr << "Auto parser error! " << l.begin << ": unknown " << err_message << std::endl;
    throw SPL_Parser::syntax_error(l, err_message);
}