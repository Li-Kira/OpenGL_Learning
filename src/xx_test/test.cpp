#include <iostream>

int main() {
    std::string s = "Hello, World!";
    const char* c = s.c_str();
    // std::cout << "Hello, World!" << std::endl;
    std::cout << c << std::endl;
}