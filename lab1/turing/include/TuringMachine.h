#pragma once

#include <cstddef>
#include <string>

#include "Alphabet.h"
#include "Head.h"
#include "Program.h"
#include "Tape.h"

class TuringMachine {
public:
    explicit TuringMachine(Alphabet alphabet);

    void loadTape(const std::string& input);

    void step();
    std::size_t run();

    const std::string& state() const noexcept;
    void setState(const std::string& state);
    bool halted() const noexcept;
    std::size_t steps() const noexcept;

    const Tape& tape() const noexcept;
    const Head& head() const noexcept;
    Program& program();

    std::string tapeString() const;

    // запрещено: Head держит ссылку на Tape этой машины
    TuringMachine(const TuringMachine&) = delete;
    TuringMachine& operator=(const TuringMachine&) = delete;

private:
    Alphabet alphabet_;
    Tape tape_;
    Head head_;
    Program program_;

    std::string state_ = "q0";
    bool halted_ = false;
    std::size_t steps_ = 0;
};
