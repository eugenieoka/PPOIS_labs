#pragma once

#include <string>

class Alphabet {
public:
    Alphabet(std::string symbols, char blank);   

    bool contains(char symbol) const noexcept;
    char blank() const noexcept;
    const std::string& symbols() const noexcept;

private:
    std::string symbols_;
    char blank_ = ' ';
};
