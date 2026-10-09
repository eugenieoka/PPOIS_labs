#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

#include "Program.h"
#include "Rule.h"

TEST(Rule, FromStateAccessor) {
    ShiftRule shift("q0");
    EXPECT_EQ(shift.fromState(), "q0");
    MoveRule move("q1", ' ', "q2", ' ', Direction::Left);
    EXPECT_EQ(move.fromState(), "q1");
}

TEST(MoveRule, MatchesExactSymbolAndState) {
    MoveRule rule("q0", ' ', "q0", ' ', Direction::Right);
    EXPECT_TRUE(rule.matches("q0", ' '));
    EXPECT_FALSE(rule.matches("q1", ' '));
    EXPECT_FALSE(rule.matches("q0", 'a'));
}

TEST(MoveRule, ApplyReturnsResult) {
    MoveRule rule("q0", ' ', "q1", 'x', Direction::Left);
    RuleResult result = rule.apply(' ');
    EXPECT_EQ(result.write, 'x');
    EXPECT_EQ(result.direction, Direction::Left);
    EXPECT_EQ(result.nextState, "q1");
    EXPECT_FALSE(result.halt);
}

TEST(ShiftRule, MatchesAnyLowercaseLetter) {
    ShiftRule rule("q0");
    EXPECT_TRUE(rule.matches("q0", 'a'));
    EXPECT_TRUE(rule.matches("q0", 'm'));
    EXPECT_TRUE(rule.matches("q0", 'z'));
    EXPECT_FALSE(rule.matches("q1", 'a'));
}

TEST(ShiftRule, DoesNotMatchNonLetters) {
    ShiftRule rule("q0");
    EXPECT_FALSE(rule.matches("q0", ' '));
    EXPECT_FALSE(rule.matches("q0", '\n'));
    EXPECT_FALSE(rule.matches("q0", '5'));
    EXPECT_FALSE(rule.matches("q0", 'A'));
}

TEST(ShiftRule, ApplyShiftsForward) {
    ShiftRule rule("q0");
    EXPECT_EQ(rule.apply('a').write, 'b');
    EXPECT_EQ(rule.apply('y').write, 'z');
    EXPECT_FALSE(rule.apply('a').halt);
    EXPECT_EQ(rule.apply('a').nextState, "q0");
    EXPECT_EQ(rule.apply('a').direction, Direction::Right);
}

TEST(ShiftRule, ApplyWrapsZToA) {
    ShiftRule rule("q0");
    EXPECT_EQ(rule.apply('z').write, 'a');
}

TEST(HaltRule, MatchesOwnSymbol) {
    HaltRule rule("q0", '\n');
    EXPECT_TRUE(rule.matches("q0", '\n'));
    EXPECT_FALSE(rule.matches("q0", ' '));
    EXPECT_FALSE(rule.matches("q1", '\n'));
}

TEST(HaltRule, ApplyHalts) {
    HaltRule rule("q0", '\n');
    RuleResult result = rule.apply('\n');
    EXPECT_TRUE(result.halt);
}

// ---------- Program ----------

TEST(Program, NewProgramIsEmpty) {
    Program program;
    EXPECT_EQ(program.size(), 0u);
    EXPECT_EQ(program.find("q0", 'a'), nullptr);
    EXPECT_TRUE(program.rules().empty());
}

TEST(Program, WrappersAddRules) {
    Program program;
    program.addShift("q0");
    program.addTransition("q0", ' ', "q0", ' ', Direction::Right);
    program.addHalt("q0", '\n');
    EXPECT_EQ(program.size(), 3u);
    EXPECT_EQ(program.rules().size(), 3u);
}

TEST(Program, FindsEachRule) {
    Program program;
    program.addShift("q0");
    program.addTransition("q0", ' ', "q0", ' ', Direction::Right);
    program.addHalt("q0", '\n');

    EXPECT_NE(program.find("q0", 'a'), nullptr);
    EXPECT_NE(program.find("q0", ' '), nullptr);
    EXPECT_NE(program.find("q0", '\n'), nullptr);
}

TEST(Program, ReturnsNullptrWhenNoMatch) {
    Program program;
    program.addShift("q0");
    EXPECT_EQ(program.find("q1", 'a'), nullptr);
    EXPECT_EQ(program.find("q0", '5'), nullptr);
}

TEST(Program, PolymorphicFindThroughBasePointer) {
    Program program;
    program.addShift("q0");
    const Rule* found = program.find("q0", 'x');
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->apply('x').write, 'y');
}

TEST(Program, AddNullThrows) {
    Program program;
    EXPECT_THROW(program.add(nullptr), std::invalid_argument);
    EXPECT_EQ(program.size(), 0u);
}
