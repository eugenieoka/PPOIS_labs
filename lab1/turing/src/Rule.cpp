#include "Rule.h"

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
