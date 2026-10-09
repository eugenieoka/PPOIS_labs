#pragma once

#include <string>

enum class Direction {
    Left,
    Right
};

struct RuleResult {
    char write;
    Direction direction;
    std::string nextState;
    bool halt = false;
};

class Rule {
public:
    explicit Rule(std::string fromState);
    virtual ~Rule() = default;

    const std::string& fromState() const noexcept;

    virtual bool matches(const std::string& state, char read) const = 0;
    virtual RuleResult apply(char read) const = 0;

protected:
    std::string fromState_;
};

class MoveRule : public Rule {
public:
    MoveRule(std::string fromState, char read, std::string toState, char write, Direction direction);

    bool matches(const std::string& state, char read) const override;
    RuleResult apply(char read) const override;

private:
    char read_;
    std::string toState_;
    char write_;
    Direction direction_;
};

class ShiftRule : public Rule {
public:
    explicit ShiftRule(std::string fromState);

    bool matches(const std::string& state, char read) const override;
    RuleResult apply(char read) const override;
};

class HaltRule : public Rule {
public:
    HaltRule(std::string fromState, char read);

    bool matches(const std::string& state, char read) const override;
    RuleResult apply(char read) const override;

private:
    char read_;
};
