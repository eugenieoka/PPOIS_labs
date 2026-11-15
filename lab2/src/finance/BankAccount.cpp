#include "transport/finance/BankAccount.h"

#include "transport/exceptions/InsufficientFundsException.h"
#include "transport/exceptions/InvalidMoneyException.h"
#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

BankAccount::BankAccount(std::string iban, Money balance)
    : iban_(std::move(iban)), balance_(std::move(balance)) {
    if (iban_.empty()) {
        throw TransportException("Bank account IBAN must not be empty");
    }
    if (balance_.isNegative()) {
        throw InvalidMoneyException("Bank account balance must not be negative");
    }
}

void BankAccount::deposit(const Money& amount) {
    if (amount.isNegative()) {
        throw InvalidMoneyException("Deposit amount must not be negative");
    }
    balance_ = balance_.add(amount);
}

void BankAccount::withdraw(const Money& amount) {
    if (amount.isNegative()) {
        throw InvalidMoneyException("Withdrawal amount must not be negative");
    }
    if (!canWithdraw(amount)) {
        throw InsufficientFundsException("Bank account balance is insufficient");
    }
    balance_ = balance_.subtract(amount);
}

void BankAccount::transferTo(BankAccount& other, const Money& amount) {
    if (this == &other) {
        throw TransportException("Cannot transfer to the same bank account");
    }
    withdraw(amount);
    other.deposit(amount);
}

bool BankAccount::canWithdraw(const Money& amount) const {
    if (amount.isNegative()) {
        throw InvalidMoneyException("Withdrawal amount must not be negative");
    }
    return balance_.amount() >= amount.amount();
}

} // namespace transport
