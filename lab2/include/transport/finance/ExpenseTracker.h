#pragma once

#include <string>
#include <utility>
#include <vector>

#include "transport/core/Money.h"

namespace transport {

class ExpenseTracker {
public:
    void addExpense(const std::string& category, const Money& amount);
    [[nodiscard]] Money total() const;
    [[nodiscard]] Money byCategory(const std::string& category) const;
    [[nodiscard]] Money largest() const;
    [[nodiscard]] std::size_t size() const noexcept { return expenses_.size(); }

private:
    std::vector<std::pair<std::string, Money>> expenses_;
};

} // namespace transport
