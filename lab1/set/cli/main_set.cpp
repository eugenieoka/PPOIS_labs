#include <iostream>
#include <string>

#include "Set.h"
#include "SetParser.h"

int main(int argc, char** argv) {
    std::string first;
    std::string second;

    if (argc >= 3) {
        first = argv[1];
        second = argv[2];
    } else {
        std::cout << "Введите первое множество: ";
        std::getline(std::cin, first);
        std::cout << "Введите второе множество: ";
        std::getline(std::cin, second);
    }

    try {
        Set a = SetParser::parse(first);
        Set b = SetParser::parse(second);

        std::cout << "A              = " << a.toString() << "\n"
                  << "B              = " << b.toString() << "\n"
                  << "A ∪ B          = " << a.unite(b).toString() << "\n"
                  << "A ∩ B          = " << a.intersect(b).toString() << "\n"
                  << "A \\ B          = " << a.difference(b).toString() << "\n"
                  << "B \\ A          = " << b.difference(a).toString() << "\n"
                  << "A △ B          = " << a.symmetricDifference(b).toString() << "\n"
                  << "A == B         : " << (a == b ? "да" : "нет") << "\n";
    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
