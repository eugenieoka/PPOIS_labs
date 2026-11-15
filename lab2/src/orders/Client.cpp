#include "transport/orders/Client.h"

#include "transport/Constants.h"
#include "transport/exceptions/InsufficientFundsException.h"
#include "transport/exceptions/InvalidClientDataException.h"
#include "transport/exceptions/InvalidMoneyException.h"

#include <utility>

namespace transport {

Client::Client(int64_t clientId, std::string fullName, Address address,
               std::string phone, Money balance)
    : clientId_(clientId),
      fullName_(std::move(fullName)),
      address_(std::move(address)),
      phone_(std::move(phone)),
      balance_(std::move(balance)) {
    if (clientId_ <= 0) {
        throw InvalidClientDataException("Client id must be positive");
    }
    if (fullName_.empty() || phone_.empty()) {
        throw InvalidClientDataException("Client name and phone must not be empty");
    }
    if (balance_.isNegative()) {
        throw InvalidClientDataException("Client balance must not be negative");
    }
}

void Client::deposit(const Money& amount) {
    if (amount.isNegative()) {
        throw InvalidMoneyException("Deposit amount must not be negative");
    }
    balance_ = balance_.add(amount);
}

void Client::withdraw(const Money& amount) {
    if (amount.isNegative()) {
        throw InvalidMoneyException("Withdrawal amount must not be negative");
    }
    if (!canPay(amount)) {
        throw InsufficientFundsException("Client balance is insufficient");
    }
    balance_ = balance_.subtract(amount);
}

bool Client::canPay(const Money& amount) const {
    if (amount.isNegative()) {
        throw InvalidMoneyException("Payment amount must not be negative");
    }
    return !blacklisted_ && !balance_.isNegative() &&
           balance_.amount() >= amount.amount();
}

void Client::addLoyaltyPoints(int points) {
    if (points < constants::kMinimumLoyaltyPoints) {
        throw InvalidClientDataException("Loyalty points must not be negative");
    }
    loyaltyPoints_ += points;
}

void Client::spendLoyaltyPoints(int points) {
    if (points < 0 || points > loyaltyPoints_) {
        throw InvalidClientDataException("Not enough loyalty points");
    }
    loyaltyPoints_ -= points;
}

double Client::discountRate() const noexcept {
    if (loyaltyPoints_ >= constants::kGoldLoyaltyThreshold) {
        return constants::kGoldDiscountRate;
    }
    if (loyaltyPoints_ >= constants::kSilverLoyaltyThreshold) {
        return constants::kSilverDiscountRate;
    }
    return 0.0;
}

void Client::block() noexcept {
    blacklisted_ = true;
}

void Client::unblock() noexcept {
    blacklisted_ = false;
}

} // namespace transport
