#include <iostream>
#include <stdexcept>
#include <string>

#include "Set.h"
#include "SetParser.h"

namespace {

void printMenu() {
    std::cout << "\n1) Ввести множество A\n"
              << "2) Ввести множество B\n"
              << "3) Показать операции над A и B\n"
              << "0) Выход\n"
              << "> " << std::flush;
}

void showOperations(const Set& a, const Set& b) {
    std::cout << "A              = " << a.toString() << "\n"
              << "B              = " << b.toString() << "\n"
              << "A ∪ B          = " << a.unite(b).toString() << "\n"
              << "A ∩ B          = " << a.intersect(b).toString() << "\n"
              << "A \\ B          = " << a.difference(b).toString() << "\n"
              << "B \\ A          = " << b.difference(a).toString() << "\n"
              << "A △ B          = " << a.symmetricDifference(b).toString() << "\n"
              << "A == B         : " << (a == b ? "да" : "нет") << "\n";
}

}  // namespace

int main() {
    Set a;
    Set b;
    bool hasA = false;
    bool hasB = false;

    std::string line;
    while (true) {
        printMenu();
        if (!std::getline(std::cin, line) || line == "0") {
            break;
        }

        if (line == "1" || line == "2") {
            const bool isA = line == "1";
            std::cout << (isA ? "Введите множество A: " : "Введите множество B: ") << std::flush;
            if (!std::getline(std::cin, line)) {
                break;
            }
            try {
                Set parsed = SetParser::parse(line);
                if (isA) {
                    a = std::move(parsed);
                    hasA = true;
                    std::cout << "A = " << a.toString() << "\n";
                } else {
                    b = std::move(parsed);
                    hasB = true;
                    std::cout << "B = " << b.toString() << "\n";
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Ошибка: " << e.what() << "\n"
                          << "Попробуйте снова.\n";
            }
        } else if (line == "3") {
            if (!hasA || !hasB) {
                std::cerr << "Сначала введите A и B (пункты 1 и 2).\n";
            } else {
                showOperations(a, b);
            }
        } else {
            std::cerr << "Неизвестный пункт меню: " << line << "\n";
        }
    }

    std::cout << "Выход.\n";
    return 0;
}
