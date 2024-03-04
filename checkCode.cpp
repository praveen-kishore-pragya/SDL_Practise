#include <iostream>
using namespace std;

void foo(int* ptr) {
    std::cout << "Function with int* argument\n";
}

void foo(char* ptr) {
    std::cout << "Function with char* argument\n";
}

int main() {
    foo(nullptr); // Resolves to the correct function based on the type of nullptr
    // foo(NULL); // Ambiguity error: ambiguous call to overloaded function
    return 0;
}