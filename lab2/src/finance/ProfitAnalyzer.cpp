#include "transport/finance/ProfitAnalyzer.h"

#include "transport/exceptions/InvalidMoneyException.h"
#include "transport/exceptions/TransportException.h"

#include <cmath>

namespace transport {

void ProfitAnalyzer::recordIncome(const Money& income) {
    if (income.isNegative()) {
        throw InvalidMoneyException("Income must not be negative");
    }
    income_ = income_.add(income);
}

void ProfitAnalyzer::addExpense(const std::string& category, const Money& amount) {
    expenses_.addExpense(category, amount);
}

Money ProfitAnalyzer::grossProfit() const {
    return income_.subtract(expenses_.total());
}

double ProfitAnalyzer::margin() const {
    if (income_.isZero()) {
        return 0.0;
    }
    return grossProfit().amount() / income_.amount() * 100.0;
}

Money ProfitAnalyzer::monthlyProfit(int months) const {
    if (months <= 0) {
        throw TransportException("Months count must be positive");
    }
    return grossProfit().multiply(1.0 / static_cast<double>(months));
}

bool ProfitAnalyzer::isProfitable() const {
    const Money profit = grossProfit();
    return !profit.isNegative() && !profit.isZero();
}

} // namespace transport
