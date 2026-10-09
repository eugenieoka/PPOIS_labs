#include <gtest/gtest.h>

#include <stdexcept>

#include "Alphabet.h"
#include "Head.h"
#include "Tape.h"

TEST(Alphabet, ContainsKnownSymbols) {
    Alphabet alpha("abcdefghijklmnopqrstuvwxyz \n", ' ');
    EXPECT_TRUE(alpha.contains('a'));
    EXPECT_TRUE(alpha.contains('z'));
    EXPECT_TRUE(alpha.contains(' '));
    EXPECT_TRUE(alpha.contains('\n'));
}

TEST(Alphabet, RejectsForeignSymbols) {
    Alphabet alpha("abcdefghijklmnopqrstuvwxyz \n", ' ');
    EXPECT_FALSE(alpha.contains('A'));
    EXPECT_FALSE(alpha.contains('5'));
    EXPECT_FALSE(alpha.contains(','));
}

TEST(Alphabet, BlankAndSymbolsAccessors) {
    Alphabet alpha("abc \n", ' ');
    EXPECT_EQ(alpha.blank(), ' ');
    EXPECT_EQ(alpha.symbols(), "abc \n");
}

TEST(Alphabet, BlankOutsideAlphabetThrows) {
    EXPECT_THROW(Alphabet("abc", '_'), std::invalid_argument);
}

TEST(Tape, ReadOnEmptyTapeReturnsBlank) {
    Tape tape(' ');
    EXPECT_EQ(tape.read(0), ' ');
    EXPECT_EQ(tape.read(42), ' ');
    EXPECT_EQ(tape.blank(), ' ');
}

TEST(Tape, WriteAndRead) {
    Tape tape(' ');
    tape.write(0, 'a');
    tape.write(1, 'b');
    EXPECT_EQ(tape.read(0), 'a');
    EXPECT_EQ(tape.read(1), 'b');
    EXPECT_EQ(tape.read(2), ' ');
}

TEST(Tape, NegativePositions) {
    Tape tape('_');
    tape.write(-5, 'x');
    EXPECT_EQ(tape.read(-5), 'x');
    EXPECT_EQ(tape.read(-4), '_');
    EXPECT_EQ(tape.blank(), '_');
}

TEST(Tape, SymbolsRange) {
    Tape tape(' ');
    tape.write(0, 'a');
    tape.write(1, 'b');
    EXPECT_EQ(tape.symbols(0, 3), "ab ");
    EXPECT_EQ(tape.symbols(-1, 2), " a");
}

TEST(Tape, PositionBounds) {
    Tape tape(' ');
    EXPECT_EQ(tape.minPosition(), 0);
    EXPECT_EQ(tape.maxPosition(), 0);
    tape.write(-3, 'x');
    tape.write(7, 'y');
    EXPECT_EQ(tape.minPosition(), -3);
    EXPECT_EQ(tape.maxPosition(), 7);
}

TEST(Tape, ClearEmptiesTape) {
    Tape tape(' ');
    tape.write(0, 'a');
    tape.clear();
    EXPECT_EQ(tape.read(0), ' ');
    EXPECT_EQ(tape.minPosition(), 0);
    EXPECT_EQ(tape.maxPosition(), 0);
}

TEST(Head, StartsAtZero) {
    Tape tape(' ');
    Head head(tape);
    EXPECT_EQ(head.position(), 0);
    EXPECT_EQ(head.read(), ' ');
}

TEST(Head, ReadWriteThroughTape) {
    Tape tape(' ');
    Head head(tape);
    head.write('a');
    EXPECT_EQ(head.read(), 'a');
    EXPECT_EQ(tape.read(0), 'a');
}

TEST(Head, MovesAcrossZero) {
    Tape tape(' ');
    Head head(tape);
    head.moveLeft();
    EXPECT_EQ(head.position(), -1);
    head.write('z');
    EXPECT_EQ(tape.read(-1), 'z');
    head.moveRight();
    head.moveRight();
    EXPECT_EQ(head.position(), 1);
}

TEST(Head, SetPosition) {
    Tape tape(' ');
    Head head(tape);
    head.setPosition(10);
    EXPECT_EQ(head.position(), 10);
    head.setPosition(0);
    EXPECT_EQ(head.position(), 0);
}
