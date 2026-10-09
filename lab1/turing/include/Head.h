#pragma once

#include "Tape.h"

class Head {
public:
    explicit Head(Tape& tape);

    char read() const;
    void write(char symbol);
    void moveLeft();
    void moveRight();

    int position() const noexcept;
    void setPosition(int position) noexcept;

private:
    Tape& tape_;
    int position_ = 0;
};
