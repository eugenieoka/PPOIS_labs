#include "transport/finance/ExpenseTracker.h"

#include "transport/exceptions/InvalidMoneyException.h"
#include "transport/exceptions/TransportException.h"

#include <cstddef>

namespace transport {

void ExpenseTracker::addExpense(const std::string& category, const Money& amount) {
    if (category.empty()) {
        throw TransportException("Expense category must not be empty");
    }
    if (amount.isNegative()) {
        throw InvalidMoneyException("Expense amount must not be negative");
    }
    expenses_.emplace_back(category, amount);
}

Money ExpenseTracker::total() const {
    Money result(0.0, "RUB");
    for (const auto& expense : expenses_) {
        result = result.add(expense.second);
    }
    return result;
}

Money ExpenseTracker::byCategory(const std::string& category) const {
    Money result(0.0, "RUB");
    for (const auto& expense : expenses_) {
        if (expense.first == category) {
            result = result.add(expense.second);
        }
    }
    return result;
}

Money ExpenseTracker::largest() const {
    if (expenses_.empty()) {
        throw TransportException("Expense tracker is empty");
    }
    Money result = expenses_.front().second;
    for (const auto& expense : expenses_) {
        if (expense.second.amount() > result.amount()) {
            result = expense.second;
        }
    }
    return result;
}

} // namespace transport
