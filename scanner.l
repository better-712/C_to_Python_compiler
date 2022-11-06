/* scanner for a toy C-like language */

%{
	#include <string>

	/* Implementation of yyFlexScanner */
	#include "scanner.hpp"
	#undef YY_DECL
	#define YY_DECL int SPL::SPL_Scanner::yylex(SPL::SPL_Parser::semantic_type * const lval, SPL::SPL_Parser::location_type *location)

	/* using "token" to make the returns for the tokens shorter to type */
	using token = SPL::SPL_Parser::token;

	/* define yyterminate as this instead of NULL */
    #define yyterminate() return( token::END )

    /* update location on matching */
    #define YY_USER_ACTION loc->step(); loc->columns(yyleng);

	using namespace std;

%}

%option nodefault
%option yylineno
%option yyclass="SPL::SPL_Scanner"
%option noyywrap
%option c++

DIGIT                   [0-9]

ID                      [a-zA-Z_][0-9a-zA-Z_]*

ILLEGAL_ID              [0-9a-zA-Z_][0-9a-zA-Z_]*

DECIMAL_INT             ([1-9]{DIGIT}{0,9}|0)
HEX_INT                 0[xX]([1-9A-Fa-f][0-9A-Fa-f]*|0)

ILLEGAL_HEX_INT         0[xX]([0-9A-Za-z]*)
DECIMAL_INT_OVERFLOW    {DIGIT}{10}{DIGIT}*

CHAR                    '.'
HEX_CHAR                '\\x[0-9A-Fa-f][0-9A-Fa-f]'

ILLEGAL_HEX_CHAR        '\\x[0-9A-Za-z]*'

FLOAT                   {DIGIT}+"."{DIGIT}*

LINE_COMMENT            \/\/[^\n\r]*(\n|\r\n|\r)?
MULTI_LINE_COMMENT      \/\*[^*]*\*+(?:[^/*][^*]*\*+)*\/

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

{DECIMAL_INT} {
	std::cout << "DECIMAL_INT" <<yylineno<< std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::INT;
}

{HEX_INT} {
	std::cout << "HEX_INT" << yylineno<< std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::INT;
}

{ILLEGAL_HEX_INT} {
	std::cout << "ILLEGAL_HEX_INT" <<yylineno<<  std::endl;
	
	yylval->build<Scan_Info*>(new Scan_Info(std::string("0"), yylineno));
    return token::INT;
}

{DECIMAL_INT_OVERFLOW} {
	std::cout << "DECIMAL_INT_OVERFLOW" <<yylineno<<  std::endl;
    printf("Error type INT at Line %d: int range overflow(%s is more than 2^32 - 1)\n",
           yylineno, yytext);
}

{CHAR}|{HEX_CHAR} {
	std::cout << "{CHAR}|{HEX_CHAR}" << yylineno<< std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::CHAR;
}

{ILLEGAL_HEX_CHAR} {
	std::cout << "{ILLEGAL_HEX_CHAR}" <<yylineno<<  std::endl;
	
	yylval->build<Scan_Info*>(new Scan_Info(std::string("'o'"), yylineno));
    return token::CHAR;
}

{FLOAT} {
	//std::cout << "FLOAT" << yylineno<< std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::FLOAT;
}

int|float|char {        // reserved type
	std::cout << "int|float|char" << yylineno<< std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::TYPE;
}

"struct" {             // reserved key word
	std::cout << "struct" << yylineno<< std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::STRUCT;
}

"if" {             // reserved key word
	std::cout << "if" <<yylineno<<  std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::IF;
}

"else" {             // reserved key word
	std::cout << "else" << yylineno<< std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::ELSE;
}

"while" {             // reserved key word
	std::cout << "while" << yylineno<< std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::WHILE;
}

"return" {             // reserved key word
	std::cout << "return" <<yylineno<<  std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::RETURN;
}

{ID} {
	std::cout << "ID" << yylineno<< std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::ID;
}

{ILLEGAL_ID} {
	std::cout << "ILLEGAL_ID" <<yylineno<<  std::endl;
	
    yylval->build<Scan_Info*>(new Scan_Info(std::string("id"), yylineno));
    return token::ID;
}

"." {
	//std::cout << "DOT" <<yylineno<<  std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::DOT;
}

";" {
	std::cout << "SEMI" <<yylineno<<  std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
	return token::SEMI;
}

"," {
	std::cout << "COMMA" <<yylineno<<  std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
	return token::COMMA;
}

"==" {
	std::cout << "EQ" << yylineno<< std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
	return token::EQ;
}

"=" {
	std::cout << "ASSIGN" <<yylineno<<  std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
	return token::ASSIGN;
}

"<=" {
	std::cout << "LE" << yylineno<< std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::LE;
}

"<" {
	std::cout << "LT" << yylineno<< std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::LT;
}

">=" {
	std::cout << "GE" <<yylineno<<  std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::GE;
}

">" {
	std::cout << "GT" <<yylineno<<  std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::GT;
}

"!=" {
	std::cout << "NE" << yylineno<< std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::NE;
}

"!" {
	std::cout << "NOT" << yylineno<< std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::NOT;
}

"+" {
	std::cout << "PLUS" << yylineno<< std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
	return token::PLUS;
}

"-" {
	std::cout << "MINUS" <<yylineno<<  std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
	return token::MINUS;
}

"*" {
	std::cout << "MUL" << yylineno<< std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
	return token::MUL;
}

"/" {
	std::cout << "DIV" <<yylineno<<  std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
	return token::DIV;
}

"&&" {
	std::cout << "AND" << yylineno<< std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
	return token::AND;
}

"||" {
	std::cout << "OR" <<yylineno<<  std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
	return token::OR;
}

"("|")" {
	std::cout << "LPRP" << yylineno<< std::endl;
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
	return yytext[0] == '(' ? token::LP : token::RP;
}

"{"|"}" {
    std::cout << "LCRC" << yylineno<< std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return yytext[0] == '{' ? token::LC : token::RC;
}

"["|"]" {
    std::cout << "LBRB" << yylineno<< std::endl;
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return yytext[0] == '[' ? token::LB : token::RB;
}

{LINE_COMMENT}|{MULTI_LINE_COMMENT} {
	yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    //return token::LINE_COMMENT;
    /* Eat up one-line_no comments */
}

[ \r\t\n]+ {
    /* Eat up one-line_no comments */
}

. {
	std::cout << "OTHER" << yylineno<< std::endl;
	
    yylval->build<Scan_Info*>(new Scan_Info(std::string(yytext), yylineno));
    return token::ERROR;
}

%%
