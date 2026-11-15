#pragma once

#include <cstdint>
#include <string>

#include "transport/core/Address.h"
#include "transport/core/Money.h"

namespace transport {

class Client {
public:
    Client(int64_t clientId, std::string fullName, Address address,
           std::string phone, Money balance = Money(0.0, "RUB"));

    [[nodiscard]] int64_t clientId() const noexcept { return clientId_; }
    [[nodiscard]] const std::string& fullName() const noexcept { return fullName_; }
    [[nodiscard]] const Address& address() const noexcept { return address_; }
    [[nodiscard]] const std::string& phone() const noexcept { return phone_; }
    [[nodiscard]] const Money& balance() const noexcept { return balance_; }
    [[nodiscard]] int loyaltyPoints() const noexcept { return loyaltyPoints_; }
    [[nodiscard]] bool isBlacklisted() const noexcept { return blacklisted_; }

    void deposit(const Money& amount);
    void withdraw(const Money& amount);
    [[nodiscard]] bool canPay(const Money& amount) const;
    void addLoyaltyPoints(int points);
    void spendLoyaltyPoints(int points);
    [[nodiscard]] double discountRate() const noexcept;
    void block() noexcept;
    void unblock() noexcept;

private:
    int64_t clientId_ = 0;
    std::string fullName_;
    Address address_;
    std::string phone_;
    Money balance_{0.0, "RUB"};
    int loyaltyPoints_ = 0;
    bool blacklisted_ = false;
};

} // namespace transport
