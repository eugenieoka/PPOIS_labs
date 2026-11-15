#pragma once

#include "transport/core/Money.h"

namespace transport {

class TaxCalculator {
public:
    TaxCalculator() = default;
    TaxCalculator(double vatRate, double incomeTaxRate);

    [[nodiscard]] double vatRate() const noexcept { return vatRate_; }
    [[nodiscard]] double incomeTaxRate() const noexcept { return incomeTaxRate_; }

    [[nodiscard]] Money vat(const Money& amount) const;
    [[nodiscard]] Money incomeTax(const Money& amount) const;
    [[nodiscard]] Money netIncome(const Money& amount) const;

private:
    double vatRate_ = 0.20;
    double incomeTaxRate_ = 0.13;

    static void validateRate(double rate);
};

} // namespace transport
