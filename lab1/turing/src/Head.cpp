#include "Head.h"

Head::Head(Tape& tape) : tape_(tape) {}

char Head::read() const {
    return tape_.read(position_);
}

void Head::write(char symbol) {
    tape_.write(position_, symbol);
}

void Head::moveLeft() {
    --position_;
}

void Head::moveRight() {
    ++position_;
}

int Head::position() const noexcept {
    return position_;
}

void Head::setPosition(int position) noexcept {
    position_ = position;
}

