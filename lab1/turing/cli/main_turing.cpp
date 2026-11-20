#include <iostream>
#include <string>
#include "TuringMachine.h"

std::string readWord(const TuringMachine& machine) {
    std::string result;
    int pos = 0;
    while (true) {
        char c = machine.tape().read(pos);
        if (c == '\n' || c == machine.tape().blank()) {
            break;
        }
        result += c;
        ++pos;
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " <файл_программы>\n";
        return 1;
    }

    try {
        auto machine = TuringMachine::loadFromFile(argv[1]);

        std::cout << "Исходная строка: " << readWord(*machine) << "\n";

        while (!machine->halted()) {
            machine->step();
        }

        std::cout << "Новая строка: " << readWord(*machine) << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}