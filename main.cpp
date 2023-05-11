//
// Created by 10578 on 10/8/2019.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>

#include "spl_driver.hpp"
#include "gen.hpp"
#include <stdio.h>
using std::cout, std::endl;

int main(const int argc, const char **argv) {
    SPL::SPL_Driver driver;
    driver.parse(argv[1]);
    if(!driver.get_errors()->empty())
        driver.print_errors();
//         SPL::print_ast(driver.get_root());
    else{
        printf("C code: \n");
        char* file=(char*)argv[1];
        FILE *fp=fopen(file,"r");
        int c;
        while((c=fgetc(fp))!=EOF)
            putchar(c);
        printf("\n");
        SPL::code_gen(driver.get_root());
    }
}
