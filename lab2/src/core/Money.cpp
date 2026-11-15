#include "transport/core/Money.h"

#include "transport/Constants.h"
#include "transport/exceptions/InvalidMoneyException.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <utility>

namespace transport {
namespace {

long long toCents(double amount) {
    if (!std::isfinite(amount)) {
        throw InvalidMoneyException("Money amount must be finite");
    }
    return static_cast<long long>(std::llround(amount * constants::kCentsPerCurrencyUnit));
}

void ensureSameCurrency(const Money& left, const Money& right) {
    if (left.currency() != right.currency()) {
        throw InvalidMoneyException("Money currencies do not match");
    }
}

} // namespace

Money::Money(double amount, std::string currency)
    : amountInCents_(toCents(amount)), currency_(std::move(currency)) {
    if (currency_.size() != 3) {
        throw InvalidMoneyException("Currency code must contain exactly three characters");
    }
}

double Money::amount() const noexcept {
    return static_cast<double>(amountInCents_) / constants::kCentsPerCurrencyUnit;
}

Money Money::add(const Money& other) const {
    ensureSameCurrency(*this, other);
    Money result;
    result.amountInCents_ = amountInCents_ + other.amountInCents_;
    result.currency_ = currency_;
    return result;
}

Money Money::subtract(const Money& other) const {
    ensureSameCurrency(*this, other);
    Money result;
    result.amountInCents_ = amountInCents_ - other.amountInCents_;
    result.currency_ = currency_;
    return result;
}

Money Money::multiply(double factor) const {
    if (!std::isfinite(factor)) {
        throw InvalidMoneyException("Money multiplier must be finite");
    }
    Money result;
    result.amountInCents_ = std::llround(static_cast<double>(amountInCents_) * factor);
    result.currency_ = currency_;
    return result;
}

bool Money::isNegative() const noexcept {
    return amountInCents_ < 0;
}

bool Money::isZero() const noexcept {
    return amountInCents_ == 0;
}

std::string Money::format() const {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << amount() << ' ' << currency_;
    return out.str();
}

bool Money::operator==(const Money& other) const noexcept {
    return amountInCents_ == other.amountInCents_ && currency_ == other.currency_;
}

} // namespace transport
