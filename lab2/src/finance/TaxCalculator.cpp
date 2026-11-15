#include "transport/finance/TaxCalculator.h"

#include "transport/exceptions/TransportException.h"

#include <cmath>

namespace transport {

TaxCalculator::TaxCalculator(double vatRate, double incomeTaxRate)
    : vatRate_(vatRate), incomeTaxRate_(incomeTaxRate) {
    validateRate(vatRate_);
    validateRate(incomeTaxRate_);
}

Money TaxCalculator::vat(const Money& amount) const {
    return amount.multiply(vatRate_);
}

Money TaxCalculator::incomeTax(const Money& amount) const {
    return amount.multiply(incomeTaxRate_);
}

Money TaxCalculator::netIncome(const Money& amount) const {
    return amount.subtract(incomeTax(amount));
}

void TaxCalculator::validateRate(double rate) {
    if (!std::isfinite(rate) || rate < 0.0 || rate > 1.0) {
        throw TransportException("Tax rate must be between 0 and 1");
    }
}

} // namespace transport
