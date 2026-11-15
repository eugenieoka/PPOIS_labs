#pragma once

#include <string>

#include "transport/core/Money.h"

namespace transport {

class Client;

class LoyaltyProgram {
public:
    LoyaltyProgram() = default;
    LoyaltyProgram(int pointsPerCurrencyUnit, int silverThreshold, int goldThreshold);

    [[nodiscard]] int pointsPerCurrencyUnit() const noexcept { return pointsPerCurrencyUnit_; }
    [[nodiscard]] int silverThreshold() const noexcept { return silverThreshold_; }
    [[nodiscard]] int goldThreshold() const noexcept { return goldThreshold_; }

    [[nodiscard]] int earn(const Money& payment) const;
    [[nodiscard]] bool canRedeem(const Client& client, int points) const noexcept;
    void redeem(Client& client, int points);
    [[nodiscard]] std::string tierFor(int points) const;

private:
    int pointsPerCurrencyUnit_ = 1;
    int silverThreshold_ = 100;
    int goldThreshold_ = 500;
};

} // namespace transport
