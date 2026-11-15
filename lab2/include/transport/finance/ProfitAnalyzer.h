#pragma once

#include "transport/core/Money.h"
#include "transport/finance/ExpenseTracker.h"

namespace transport {

class ProfitAnalyzer {
public:
    void recordIncome(const Money& income);
    void addExpense(const std::string& category, const Money& amount);
    [[nodiscard]] Money grossProfit() const;
    [[nodiscard]] double margin() const;
    [[nodiscard]] Money monthlyProfit(int months) const;
    [[nodiscard]] bool isProfitable() const;

private:
    Money income_{0.0, "RUB"};
    ExpenseTracker expenses_;
};

} // namespace transport
