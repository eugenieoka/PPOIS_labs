#include <iostream>
#include <string>

#include "Alphabet.h"
#include "TuringMachine.h"

namespace {

const std::string kAlphabet = "abcdefghijklmnopqrstuvwxyz \n";
const char kBlank = ' ';

void buildProgram(TuringMachine& machine) {
    machine.program().addShift("q0");
    machine.program().addTransition("q0", ' ', "q0", kBlank, Direction::Right);
    machine.program().addHalt("q0", '\n');
}

std::string readResult(const TuringMachine& machine, std::size_t inputLength) {
    std::string result;
    for (std::size_t i = 0; i < inputLength; ++i) {
        result += machine.tape().read(static_cast<int>(i));
    }
    return result;
}

} 

int main() {
    std::cout << "введите строку (только a-z и пробелы), пустая строка — выход\n";

    TuringMachine machine(Alphabet(kAlphabet, kBlank));
    buildProgram(machine);

    std::string line;
    while (true) {
        std::cout << "> " << std::flush;
        if (!std::getline(std::cin, line) || line.empty()) {
            break;
        }

        try {
            machine.loadTape(line);
            machine.run();

            std::cout << "  Вход : " << line << "\n"
                      << "  Выход: " << readResult(machine, line.size()) << "\n"
                      << "  Шагов: " << machine.steps() << ", состояние: " << machine.state()
                      << (machine.halted() ? " (остановлена)" : "") << "\n";
        } catch (const std::invalid_argument& e) {
            std::cerr << "  Ошибка: " << e.what() << "\n"
                      << "  Попробуйте снова.\n";
        }
    }

    std::cout << "Выход.\n";
    return 0;
}
