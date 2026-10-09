#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>

#include "Alphabet.h"
#include "TuringMachine.h"

namespace {

Alphabet englishAlphabet() {
    return Alphabet("abcdefghijklmnopqrstuvwxyz \n", ' ');
}

class MachineTest : public ::testing::Test {
protected:
    void SetUp() override {
        machine = std::make_unique<TuringMachine>(englishAlphabet());
        machine->program().addShift("q0");
        machine->program().addTransition("q0", ' ', "q0", ' ', Direction::Right);
        machine->program().addHalt("q0", '\n');
    }

    std::string readRange(int from, int count) {
        std::string out;
        for (int i = 0; i < count; ++i) {
            out += machine->tape().read(from + i);
        }
        return out;
    }

    std::unique_ptr<TuringMachine> machine;
};

} 

TEST_F(MachineTest, InitialState) {
    EXPECT_EQ(machine->state(), "q0");
    EXPECT_FALSE(machine->halted());
    EXPECT_EQ(machine->steps(), 0u);
}

TEST_F(MachineTest, LoadTapeWritesInputAndTerminator) {
    machine->loadTape("ab");
    EXPECT_EQ(machine->tapeString(), "|ab↵|");
    EXPECT_EQ(machine->head().position(), 0);
    EXPECT_EQ(machine->tape().read(2), '\n');
}

TEST_F(MachineTest, StepWritesMovesAndCounts) {
    machine->loadTape("ab");
    machine->step();
    EXPECT_EQ(machine->steps(), 1u);
    EXPECT_FALSE(machine->halted());
    EXPECT_EQ(machine->tape().read(0), 'b');
    EXPECT_EQ(machine->head().position(), 1);
    EXPECT_EQ(machine->state(), "q0");
}

TEST_F(MachineTest, RunShiftsWholeString) {
    machine->loadTape("hello");
    std::size_t steps = machine->run();
    EXPECT_TRUE(machine->halted());
    EXPECT_EQ(readRange(0, 5), "ifmmp");
    EXPECT_EQ(steps, 6u);
    EXPECT_EQ(machine->state(), "q0");
}

TEST_F(MachineTest, SpacesArePreserved) {
    machine->loadTape("hello world");
    machine->run();
    EXPECT_EQ(readRange(0, 11), "ifmmp xpsme");
    EXPECT_EQ(machine->steps(), 12u);
}

TEST_F(MachineTest, ZWrapsToA) {
    machine->loadTape("xyz");
    machine->run();
    EXPECT_EQ(readRange(0, 3), "yza");
}

TEST_F(MachineTest, EmptyInputHaltsImmediately) {
    machine->loadTape("");
    machine->run();
    EXPECT_TRUE(machine->halted());
    EXPECT_EQ(machine->steps(), 1u);
}

TEST_F(MachineTest, StepAfterHaltDoesNothing) {
    machine->loadTape("a");
    machine->run();
    std::size_t before = machine->steps();
    machine->step();
    EXPECT_EQ(machine->steps(), before);
}

TEST_F(MachineTest, RejectsSymbolsOutsideAlphabet) {
    EXPECT_THROW(machine->loadTape("Hi"), std::invalid_argument);
    EXPECT_THROW(machine->loadTape("123"), std::invalid_argument);
    EXPECT_THROW(machine->loadTape("a,b"), std::invalid_argument);
}

TEST_F(MachineTest, RejectsNewlineInInput) {
    EXPECT_THROW(machine->loadTape("a\nb"), std::invalid_argument);
}

TEST_F(MachineTest, MissingRuleHaltsInsteadOfThrowing) {
    TuringMachine m(englishAlphabet());
    m.program().addShift("q0");  // без правила для пробела и терминатора
    m.loadTape("ab");
    m.run();
    EXPECT_TRUE(m.halted());
    EXPECT_EQ(m.steps(), 3u);
    EXPECT_EQ(m.tape().read(0), 'b');
    EXPECT_EQ(m.tape().read(1), 'c');
}

TEST_F(MachineTest, EmptyProgramHaltsOnFirstStep) {
    TuringMachine m(englishAlphabet());
    m.loadTape("a");
    m.run();
    EXPECT_TRUE(m.halted());
    EXPECT_EQ(m.steps(), 1u);
    EXPECT_EQ(m.tape().read(0), 'a');
}

TEST_F(MachineTest, LeftDirectionMovesHeadBackwards) {
    TuringMachine m(englishAlphabet());
    m.program().addTransition("q0", 'a', "q1", 'b', Direction::Left);
    m.loadTape("a");
    m.step();
    EXPECT_EQ(m.head().position(), -1);
    EXPECT_EQ(m.tape().read(0), 'b');
    EXPECT_EQ(m.state(), "q1");
}

TEST_F(MachineTest, ReloadResetsEverything) {
    machine->loadTape("a");
    machine->run();
    EXPECT_TRUE(machine->halted());
    EXPECT_EQ(machine->steps(), 2u);

    machine->loadTape("b");
    EXPECT_FALSE(machine->halted());
    EXPECT_EQ(machine->steps(), 0u);
    EXPECT_EQ(machine->state(), "q0");
}

TEST_F(MachineTest, SetStateIsApplied) {
    machine->setState("q1");
    EXPECT_EQ(machine->state(), "q1");
    machine->setState("q0");
    EXPECT_EQ(machine->state(), "q0");
}

TEST_F(MachineTest, TapeStringEscapesNewline) {
    machine->loadTape("ab");
    EXPECT_EQ(machine->tapeString(), "|ab↵|");
    machine->loadTape("");
    EXPECT_EQ(machine->tapeString(), "|↵|");
}
