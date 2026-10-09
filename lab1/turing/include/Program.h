#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Rule.h"

class Program {
public:
    void add(std::unique_ptr<Rule> rule);

    void addTransition(const std::string& from, char read, const std::string& to, char write,
                       Direction dir);
    void addShift(const std::string& from);
    void addHalt(const std::string& from, char read);

    const Rule* find(const std::string& state, char read) const;

    std::size_t size() const noexcept;
    const std::vector<std::unique_ptr<Rule>>& rules() const;

private:
    std::vector<std::unique_ptr<Rule>> rules_;
};
