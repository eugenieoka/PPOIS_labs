#pragma once

#include <map>
#include <string>

class Tape {
public:
    explicit Tape(char blank);

    char read(int position) const;
    void write(int position, char symbol);

    char blank() const noexcept;
    void clear() noexcept;

    std::string symbols(int from, int count) const;
    int minPosition() const noexcept;
    int maxPosition() const noexcept;

private:
    char blank_;
    std::map<int, char> cells_;
};

