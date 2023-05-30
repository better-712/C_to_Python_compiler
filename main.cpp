//
// Created by 10578 on 10/8/2019.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <chrono>
#include <sys/resource.h>
#include <sys/time.h>

#include "spl_driver.hpp"
#include "gen.hpp"
#include "semantics.hpp"
#include <stdio.h>

using std::cout, std::endl;

int main(const int argc, const char **argv) {
    SPL::SPL_Driver driver;
    driver.parse(argv[1]);
    

    //SPL::semantic_analyze(driver.get_root());
   
    
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
     struct rusage usage;
    
    getrusage(RUSAGE_SELF, &usage);
    long int memory_usage = usage.ru_maxrss;
    printf("Memory usage: %ld KB\n", memory_usage/1024);
    struct timeval user_time = usage.ru_utime;
    struct timeval sys_time = usage.ru_stime;
    printf("User CPU time: %d seconds %d microseconds\n", (int)user_time.tv_sec, (int)user_time.tv_usec);
    return 0;
}
