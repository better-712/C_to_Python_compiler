//
// Created by 10578 on 10/9/2019.
//

#include <cctype>
#include <fstream>
#include <cassert>
#include <algorithm>

#include "spl_driver.hpp"


namespace SPL {

    SPL_Driver::~SPL_Driver() {
        delete (scanner);
        scanner = nullptr;
        delete (parser);
        parser = nullptr;
    }

    void SPL_Driver::parse(const char *filename) {
        assert(filename != nullptr);
        std::ifstream in_file(filename);
        parse_helper(in_file);
    }

    

    void SPL_Driver::parse_helper(std::istream &is) {
        delete (scanner);
        
        scanner = new SPL_Scanner(&is);
        
        delete (parser);
        
        parser = new SPL_Parser(*scanner, *this);

        parser->parse();
        
    }
    void SPL_Driver::set_root(Node *node){
        root = node;
    }
    Node* SPL_Driver::get_root(){
        return root;
    }
    void SPL_Driver::add_syntax_error(std::string lexeme, Scan_Info *info) {
        syntax_errors->push_back(new Syntax_Error(lexeme, info->line_no));
    }
}
