#pragma once

#include "transport/core/Money.h"

namespace transport {

class Salary {
public:
    Salary() = default;
    Salary(Money base, Money bonus, Money deductions);

    [[nodiscard]] const Money& base() const noexcept { return base_; }
    [[nodiscard]] const Money& bonus() const noexcept { return bonus_; }
    [[nodiscard]] const Money& deductions() const noexcept { return deductions_; }

    [[nodiscard]] Money net() const;
    void addBonus(const Money& amount);
    void addDeduction(const Money& amount);
    [[nodiscard]] bool isPositive() const;

private:
    Money base_{0.0, "RUB"};
    Money bonus_{0.0, "RUB"};
    Money deductions_{0.0, "RUB"};
};

} // namespace transport
