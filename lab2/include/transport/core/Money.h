#pragma once

#include <string>

namespace transport {

class Money {
public:
    Money() = default;
    explicit Money(double amount, std::string currency = "RUB");

    [[nodiscard]] double amount() const noexcept;
    [[nodiscard]] const std::string& currency() const noexcept { return currency_; }

    [[nodiscard]] Money add(const Money& other) const;
    [[nodiscard]] Money subtract(const Money& other) const;
    [[nodiscard]] Money multiply(double factor) const;
    [[nodiscard]] bool isNegative() const noexcept;
    [[nodiscard]] bool isZero() const noexcept;
    [[nodiscard]] std::string format() const;

    bool operator==(const Money& other) const noexcept;

private:
    long long amountInCents_ = 0;
    std::string currency_ = "RUB";
};

} // namespace transport
