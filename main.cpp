//
// Created by 10578 on 10/8/2019.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>

#include "spl_driver.hpp"

using std::cout, std::endl;

int main(const int argc, const char **argv) {
    if (argc == 2) {
        SPL::SPL_Driver driver;

        if (std::strncmp(argv[1], "-o", 2) == 0) {
            driver.parse(std::cin);
        } else if (std::strncmp(argv[1], "-h", 2) == 0) {
            cout << "use -o for pipe to std::cin" << endl;
            cout << "just give a filename to count from a file" << endl;
            cout << "use -h to get this menu" << endl;
            return EXIT_SUCCESS;
        } else {
            driver.parse(argv[1]);
        }

        if (driver.error_reported()) {
            driver.print_errors();
        } else {
            SPL::print_ast(driver.get_root());
        }
    } else {
        cout << "Only one parameter permitted! Use -h to see usage." << endl;
        return EXIT_FAILURE;
    }
}
