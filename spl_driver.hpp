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

#include "scanner.hpp"
#include "parser.tab.hpp"

namespace SPL {

    class SPL_Driver {
    public:
        SPL_Driver() : parser{nullptr}, scanner{nullptr}{};

        virtual ~SPL_Driver();

        void parse(const char *filename);


    private:
        void parse_helper(std::istream &is);

        SPL::SPL_Parser *parser = nullptr;
        SPL::SPL_Scanner *scanner = nullptr;
        
    };

    
}

#endif //END ___SPL_DRIVER_HPP__
