#include "Rule.h"

#include <cctype>
#include <utility>

Rule::Rule(std::string fromState) : fromState_(std::move(fromState)) {}

const std::string& Rule::fromState() const noexcept {
    return fromState_;
}

MoveRule::MoveRule(std::string fromState, char read, std::string toState, char write,
                   Direction direction)
    : Rule(std::move(fromState)),
      read_(read),
      toState_(std::move(toState)),
      write_(write),
      direction_(direction) {}

bool MoveRule::matches(const std::string& state, char read) const {
    return state == fromState_ && read == read_;
}

RuleResult MoveRule::apply(char) const {
    return RuleResult{write_, direction_, toState_, false};
}

ShiftRule::ShiftRule(std::string fromState) : Rule(std::move(fromState)) {}

bool ShiftRule::matches(const std::string& state, char read) const {
    return state == fromState_ && std::islower(static_cast<unsigned char>(read));
}

RuleResult ShiftRule::apply(char read) const {
    char next = (read == 'z') ? 'a' : static_cast<char>(read + 1);
    return RuleResult{next, Direction::Right, fromState_, false};
}

HaltRule::HaltRule(std::string fromState, char read)
    : Rule(std::move(fromState)), read_(read) {}

bool HaltRule::matches(const std::string& state, char read) const {
    return state == fromState_ && read == read_;
}

RuleResult HaltRule::apply(char) const {
    return RuleResult{read_, Direction::Right, fromState_, true};
}
