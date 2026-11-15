#include "transport/staff/Salary.h"

#include "transport/exceptions/InvalidMoneyException.h"

#include <utility>

namespace transport {

Salary::Salary(Money base, Money bonus, Money deductions)
    : base_(std::move(base)), bonus_(std::move(bonus)), deductions_(std::move(deductions)) {
    if (base_.isNegative() || bonus_.isNegative() || deductions_.isNegative()) {
        throw InvalidMoneyException("Salary components must not be negative");
    }
    if (deductions_.amount() > base_.amount() + bonus_.amount()) {
        throw InvalidMoneyException("Deductions must not exceed base salary and bonus");
    }
}

Money Salary::net() const {
    return base_.add(bonus_).subtract(deductions_);
}

void Salary::addBonus(const Money& amount) {
    if (amount.isNegative()) {
        throw InvalidMoneyException("Bonus must not be negative");
    }
    bonus_ = bonus_.add(amount);
}

void Salary::addDeduction(const Money& amount) {
    if (amount.isNegative()) {
        throw InvalidMoneyException("Deduction must not be negative");
    }
    const Money updated = deductions_.add(amount);
    if (updated.amount() > base_.amount() + bonus_.amount()) {
        throw InvalidMoneyException("Deductions must not exceed base salary and bonus");
    }
    deductions_ = updated;
}

bool Salary::isPositive() const {
    const Money result = net();
    return !result.isNegative() && !result.isZero();
}

} // namespace transport
