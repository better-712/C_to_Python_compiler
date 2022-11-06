#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "utils.hpp"

using namespace std;


class A {
public:
    int i;
};

class B : public A {
public:
    void print_i() { cout << i; }
};

int value = 0;

int &r_get() {
    return value;
}



int main() {
    const char *s = "-10";
    int a = atoi(s);

    cout << a << endl;

    const char *c = "'\x7E'";
    char ch = (char) c[1];
    char trimmed = SPL::trim(c, "'").c_str()[0];
    cout << ch << endl;

    A b = B();
    b.i = 10;
    cout << b.i << endl;

    r_get() = 2;
    cout << value << endl;

    return 0;
}
