#pragma once

#include <string>

#include "transport/core/Money.h"

namespace transport {

class BankAccount {
public:
    BankAccount() = default;
    BankAccount(std::string iban, Money balance);

    [[nodiscard]] const std::string& iban() const noexcept { return iban_; }
    [[nodiscard]] const Money& balance() const noexcept { return balance_; }

    void deposit(const Money& amount);
    void withdraw(const Money& amount);
    void transferTo(BankAccount& other, const Money& amount);
    [[nodiscard]] bool canWithdraw(const Money& amount) const;

private:
    std::string iban_;
    Money balance_{0.0, "RUB"};
};

} // namespace transport
