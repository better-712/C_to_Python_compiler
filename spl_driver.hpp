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
        SPL_Driver() : parser{nullptr}, scanner{nullptr}{};

        virtual ~SPL_Driver();

        void parse(const char *filename);
        void set_root(SPL::Node *node);
        SPL::Node *get_root();
        
        void add_syntax_error(std::string lexeme, Scan_Info *info);
        
        std::vector<Error *> *syntax_errors=new std::vector<Error *>(){};
        std::vector<Error *> *get_errors();
        void print_errors();

    private:
        void parse_helper(std::istream &is);

        SPL::SPL_Parser *parser = nullptr;
        SPL::SPL_Scanner *scanner = nullptr;
        SPL::Node *root = nullptr;
    };

    
}

#endif //END ___SPL_DRIVER_HPP__
