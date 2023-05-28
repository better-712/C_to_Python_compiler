//
// Created by 10578 on 10/8/2019.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <chrono>

#include "spl_driver.hpp"
#include "gen.hpp"
#include "semantics.hpp"
#include <stdio.h>

using std::cout, std::endl;

int main(const int argc, const char **argv) {
    SPL::SPL_Driver driver;
    driver.parse(argv[1]);
    auto start = std::chrono::high_resolution_clock::now();

   
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
     // 结束计时
    auto end = std::chrono::high_resolution_clock::now();

    // 计算耗时（以毫秒为单位）
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // 输出运行时间
    std::cout << "程序运行时间：" << duration << "毫秒" << std::endl;
}
