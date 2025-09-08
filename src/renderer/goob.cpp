#include <iostream>
#include "goob.hpp"

void Goob::hello() {
    std::cout << generate_hello() << std::endl;
}

std::string Goob::generate_hello() {
    return "Hello World!";
} 