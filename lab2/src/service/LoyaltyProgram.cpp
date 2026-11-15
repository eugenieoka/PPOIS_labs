#include "transport/service/LoyaltyProgram.h"

#include "transport/exceptions/InvalidClientDataException.h"
#include "transport/exceptions/InvalidMoneyException.h"
#include "transport/exceptions/TransportException.h"
#include "transport/orders/Client.h"

#include <cmath>

namespace transport {

LoyaltyProgram::LoyaltyProgram(int pointsPerCurrencyUnit, int silverThreshold, int goldThreshold)
    : pointsPerCurrencyUnit_(pointsPerCurrencyUnit),
      silverThreshold_(silverThreshold),
      goldThreshold_(goldThreshold) {
    if (pointsPerCurrencyUnit_ <= 0) {
        throw TransportException("Loyalty points per currency unit must be positive");
    }
    if (silverThreshold_ <= 0 || goldThreshold_ <= silverThreshold_) {
        throw TransportException("Loyalty tiers are invalid");
    }
}

int LoyaltyProgram::earn(const Money& payment) const {
    if (payment.isNegative()) {
        throw InvalidMoneyException("Payment must not be negative");
    }
    return static_cast<int>(std::floor(payment.amount())) * pointsPerCurrencyUnit_;
}

bool LoyaltyProgram::canRedeem(const Client& client, int points) const noexcept {
    return points >= 0 && client.loyaltyPoints() >= points;
}

void LoyaltyProgram::redeem(Client& client, int points) {
    if (!canRedeem(client, points)) {
        throw InvalidClientDataException("Client does not have enough loyalty points");
    }
    client.spendLoyaltyPoints(points);
}

std::string LoyaltyProgram::tierFor(int points) const {
    if (points >= goldThreshold_) {
        return "Gold";
    }
    if (points >= silverThreshold_) {
        return "Silver";
    }
    return "Standard";
}

} // namespace transport
