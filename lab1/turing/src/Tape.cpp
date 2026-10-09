#include "Tape.h"

Tape::Tape(char blank) : blank_(blank) {}

char Tape::read(int position) const {
    auto it = cells_.find(position);
    return it == cells_.end() ? blank_ : it->second;
}

void Tape::write(int position, char symbol) {
    cells_[position] = symbol;
}

char Tape::blank() const noexcept {
    return blank_;
}

void Tape::clear() noexcept {
    cells_.clear();
}

std::string Tape::symbols(int from, int count) const {
    std::string result;
    result.reserve(static_cast<std::size_t>(count));
    for (int i = 0; i < count; ++i) {
        result += read(from + i);
    }
    return result;
}

int Tape::minPosition() const noexcept {
    return cells_.empty() ? 0 : cells_.begin()->first;
}

int Tape::maxPosition() const noexcept {
    return cells_.empty() ? 0 : cells_.rbegin()->first;
}
