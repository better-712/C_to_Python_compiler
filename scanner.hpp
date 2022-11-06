//
// Created by 10578 on 10/9/2019.
//

#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#if !defined(yyFlexLexerOnce)

#include <FlexLexer.h>

#endif

#include <unordered_map>
#include "parser.tab.hpp"
#include "location.hh"

namespace SPL {

    class Scan_Info {
    public:
        std::string lexeme;
        int line_no;

        Scan_Info(std::string lexeme, int line) : lexeme{lexeme}, line_no{line} {};
    };

    class Error {
    public:
        Scan_Info *info{};

        virtual std::string to_string() = 0;

        bool operator<(const Error &obj) {
            return info->line_no < obj.info->line_no;
        }
    };

    class Lexical_Error : public Error {
    public:
        explicit Lexical_Error(Scan_Info *info) {
            this->info = info;
        };

        std::string to_string() override {
            return "Error type A at Line " + std::to_string(info->line_no) + ": unknown lexeme " + info->lexeme;
        }
    };

    class Syntax_Error : public Error {
    public:
        std::unordered_map<std::string, std::string> punctuation_map{
                {";", "semicolon"},
                {",", "comma"},
                {")", "closing parenthesis"},
                {"]", "closing bracket"},
                {"}", "closing curly bracket"},
        };

        explicit Syntax_Error(Scan_Info *info) {
            this->info = info;
        };

        Syntax_Error(std::string lexeme, int line_no) {
            this->info = new Scan_Info(lexeme, line_no);
        };

        std::string to_string() override {
            return "Error type B at Line " + std::to_string(info->line_no) +
                   ": Missing " + punctuation_map[info->lexeme] + " " + info->lexeme;
        }
    };

    class SPL_Scanner : public yyFlexLexer {
    public:

        SPL_Scanner(std::istream *in) : yyFlexLexer(in), loc{new SPL::SPL_Parser::location_type()},
                                        lexical_errors{new std::vector<Error *>{}} {
        };

        //get rid of override virtual function warning
        using FlexLexer::yylex;

        virtual int yylex(SPL::SPL_Parser::semantic_type *const lval,
                          SPL::SPL_Parser::location_type *location);
        // YY_DECL defined in scanner.l
        // Method body created by flex in scanner.yy.cpp

        void add_error(Scan_Info *info) {
            lexical_errors->push_back(new Lexical_Error(info));
        };

        bool error_reported() {
            return !lexical_errors->empty();
        }

        std::vector<Error *> *get_errors() {
            return lexical_errors;
        }

    private:
        /* yyval ptr */
        SPL::SPL_Parser::semantic_type *yylval = nullptr;
        /* location ptr */
        SPL::SPL_Parser::location_type *loc = nullptr;

        std::vector<Error *> *lexical_errors = nullptr;
    };

} /* end namespace SPL */

#endif // END __SCANNER_HPP__