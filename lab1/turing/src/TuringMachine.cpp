#include "TuringMachine.h"

#include <stdexcept>
#include <utility>
#include <fstream>
#include <sstream>


TuringMachine::TuringMachine(Alphabet alphabet)
    : alphabet_(std::move(alphabet)), tape_(alphabet_.blank()), head_(tape_) {}

void TuringMachine::loadTape(const std::string& input) {
    if (input.find('\n') != std::string::npos) {
        throw std::invalid_argument("TuringMachine::loadTape: newline is reserved as terminator");
    }
    for (char c : input) {
        if (!alphabet_.contains(c)) {
            throw std::invalid_argument(std::string("TuringMachine::loadTape: symbol '") + c + "' is not in the alphabet");
        }
    }

    tape_.clear();
    head_.setPosition(0);
    state_ = "q0";
    halted_ = false;
    steps_ = 0;

    for (std::size_t i = 0; i < input.size(); ++i) {
        tape_.write(static_cast<int>(i), input[i]);
    }
    tape_.write(static_cast<int>(input.size()), '\n');
}

void TuringMachine::step() {
    if (halted_) {
        return;
    }
    ++steps_;

    char read = head_.read();
    const Rule* rule = program_.find(state_, read);
    if (!rule) {             
        halted_ = true;
        return;
    }

    RuleResult result = rule->apply(read);
    if (result.halt) {
        halted_ = true;
        return;
    }

    head_.write(result.write);
    if (result.direction == Direction::Right) {
        head_.moveRight();
    } else {
        head_.moveLeft();
    }
    state_ = result.nextState;
}

std::size_t TuringMachine::run() {
    while (!halted_) {
        step();
    }
    return steps_;
}

const std::string& TuringMachine::state() const noexcept {
    return state_;
}

void TuringMachine::setState(const std::string& state) {
    state_ = state;
}

bool TuringMachine::halted() const noexcept {
    return halted_;
}

std::size_t TuringMachine::steps() const noexcept {
    return steps_;
}

const Tape& TuringMachine::tape() const noexcept {
    return tape_;
}

const Head& TuringMachine::head() const noexcept {
    return head_;
}

Program& TuringMachine::program() {
    return program_;
}

std::string TuringMachine::tapeString() const {
    std::string result = "|";
    for (int i = tape_.minPosition(); i <= tape_.maxPosition(); ++i) {
        char c = tape_.read(i);
        if (c == '\n') {
            result += "↵";
        } else {
            result += c;
        }
    }
    result += "|";
    return result;
}

namespace {

char parseSymbolToken(const std::string& token) {
    if (token == "_") {
        return ' ';
    }
    if (token == "/n" || token == "\\n") {
        return '\n';
    }
    return token.empty() ? '\0' : token.front();
}

}  

std::unique_ptr<TuringMachine> TuringMachine::loadFromFile(const std::string& program) {
    std::ifstream file(program);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + program);
    }

    const std::string alphabetPrefix = "alphabet: ";
    const std::string tapePrefix = "tape: ";

    std::string line;
    std::string symbols;
    std::string tapeData;

    std::getline(file, line);
    if (line.compare(0, alphabetPrefix.size(), alphabetPrefix) == 0) {
        symbols = line.substr(alphabetPrefix.size());
        symbols += " \n";
    } else {
        throw std::runtime_error("Ошибка формата: первая строка должна быть 'alphabet: ...'");
    }

    std::getline(file, line);
    if (line.compare(0, tapePrefix.size(), tapePrefix) == 0) {
        tapeData = line.substr(tapePrefix.size());
    } else {
        throw std::runtime_error("Ошибка формата: вторая строка должна быть 'tape: ...'");
    }

    auto machine = std::make_unique<TuringMachine>(Alphabet(symbols, ' '));
    machine->loadTape(tapeData);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string from, readStr, to, writeStr, dirStr;
        ss >> from >> readStr >> to >> writeStr >> dirStr;

        char readC = parseSymbolToken(readStr);
        char writeC = parseSymbolToken(writeStr);

        if (dirStr == "S") {
            machine->program().addHalt(from, readC);
        } else {
            Direction dir = (dirStr == "L") ? Direction::Left : Direction::Right;
            machine->program().addTransition(from, readC, to, writeC, dir);
        }
    }

    return machine;
}
