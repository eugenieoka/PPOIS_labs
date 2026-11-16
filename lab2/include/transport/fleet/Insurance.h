#pragma once

#include <string>

#include "transport/core/Money.h"
#include "transport/core/TransportDate.h"

namespace transport {

class Insurance {
public:
    Insurance() = default;
    Insurance(std::string policyNumber, std::string vehiclePlate,
              Money premium, TransportDate validFrom, TransportDate validTo);

    [[nodiscard]] const std::string& policyNumber() const noexcept { return policyNumber_; }
    [[nodiscard]] const std::string& vehiclePlate() const noexcept { return vehiclePlate_; }
    [[nodiscard]] const Money& premium() const noexcept { return premium_; }
    [[nodiscard]] const TransportDate& validFrom() const noexcept { return validFrom_; }
    [[nodiscard]] const TransportDate& validTo() const noexcept { return validTo_; }

    [[nodiscard]] bool isActiveOn(const TransportDate& date) const noexcept;
    void renew(const TransportDate& newValidTo, const Money& newPremium);
    [[nodiscard]] const Money& annualCost() const noexcept;

private:
    std::string policyNumber_;
    std::string vehiclePlate_;
    Money premium_{0.0, "RUB"};
    TransportDate validFrom_;
    TransportDate validTo_;
};

} // namespace transport
