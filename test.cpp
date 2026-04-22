#include "src/include/int2048.h"
#include <iostream>

using namespace sjtu;

int main() {
    // Test basic operations
    int2048 a("12345678901234567890");
    int2048 b("98765432109876543210");

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    int2048 c = a + b;
    std::cout << "a + b = " << c << std::endl;

    int2048 d = b - a;
    std::cout << "b - a = " << d << std::endl;

    int2048 e = a * b;
    std::cout << "a * b = " << e << std::endl;

    int2048 f = b / a;
    std::cout << "b / a = " << f << std::endl;

    int2048 g = b % a;
    std::cout << "b % a = " << g << std::endl;

    // Test negative numbers
    int2048 h("-12345678901234567890");
    std::cout << "h = " << h << std::endl;

    int2048 i = a + h;
    std::cout << "a + h = " << i << std::endl;

    // Test comparison
    std::cout << "a < b: " << (a < b) << std::endl;
    std::cout << "a == a: " << (a == a) << std::endl;

    return 0;
}