//
// Created by 10578 on 10/9/2019.
//

#ifndef ___SPL_DRIVER_HPP__
#define ___SPL_DRIVER_HPP__

#include <string>
#include <cstddef>
#include <istream>
#include <unordered_map>
#include <algorithm>
#include <initializer_list>

#include "ast.hpp"
#include "scanner.hpp"
#include "parser.tab.hpp"

namespace SPL {

    class SPL_Driver {
    public:
        SPL_Driver() : parser{nullptr}, scanner{nullptr}, root{nullptr}, syntax_errors{new std::vector<Error *>{}} {};

        virtual ~SPL_Driver();

        /**
         * Parse from a file.
         * @param filename - string of input file
         */
        void parse(const char *filename);

        void parse(std::istream &is);

        void set_root(SPL::Program_Node *node);

        SPL::Program_Node *get_root();

        SPL_Scanner *get_scanner();

        std::vector<Error *> *get_errors();

        void add_syntax_error(std::string lexeme, Scan_Info *info);

        void add_syntax_error(std::string lexeme, AST_Node *node);

        bool error_reported();

        void print_errors();

    private:
        void parse_helper(std::istream &is);

        SPL::SPL_Parser *parser = nullptr;
        SPL::SPL_Scanner *scanner = nullptr;

        SPL::Program_Node *root = nullptr;

        std::vector<Error *> *syntax_errors;

        /* define some pretty colors */
        const std::string red = "\033[1;31m";
        const std::string blue = "\033[1;36m";
        const std::string norm = "\033[0m";
    };

    /* Useful functions */

//    std::vector<Scan_Info *> make_leaves(std::initializer_list<Scan_Info *> terminal_leaves);

}

#endif //END ___SPL_DRIVER_HPP__
