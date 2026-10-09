#include "Program.h"

#include <stdexcept>
#include <utility>

void Program::add(std::unique_ptr<Rule> rule) {
    if (!rule) {
        throw std::invalid_argument("Program::add: rule is null");
    }
    rules_.push_back(std::move(rule));
}

void Program::addTransition(const std::string& from, char read, const std::string& to, char write,
                            Direction dir) {
    add(std::make_unique<MoveRule>(from, read, to, write, dir));
}

void Program::addShift(const std::string& from) {
    add(std::make_unique<ShiftRule>(from));
}

void Program::addHalt(const std::string& from, char read) {
    add(std::make_unique<HaltRule>(from, read));
}

const Rule* Program::find(const std::string& state, char read) const {
    for (const std::unique_ptr<Rule>& rule : rules_) {
        if (rule->matches(state, read)) {
            return rule.get();
        }
    }
    return nullptr;
}

std::size_t Program::size() const noexcept {
    return rules_.size();
}

const std::vector<std::unique_ptr<Rule>>& Program::rules() const {
    return rules_;
}
