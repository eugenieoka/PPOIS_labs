#include "Alphabet.h"

#include <stdexcept>
#include <utility>

Alphabet::Alphabet(std::string symbols, char blank)
    : symbols_(std::move(symbols)), blank_(blank) {
    if (symbols_.find(blank) == std::string::npos) {
        throw std::invalid_argument("Alphabet: blank symbol is not part of the alphabet");
    }
}

bool Alphabet::contains(char symbol) const noexcept {
    return symbols_.find(symbol) != std::string::npos;
}

char Alphabet::blank() const noexcept {
    return blank_;
}

const std::string& Alphabet::symbols() const noexcept {
    return symbols_;
}
