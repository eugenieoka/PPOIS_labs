#include "transport/fleet/Insurance.h"

#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

Insurance::Insurance(std::string policyNumber, std::string vehiclePlate,
                     Money premium, TransportDate validFrom, TransportDate validTo)
    : policyNumber_(std::move(policyNumber)),
      vehiclePlate_(std::move(vehiclePlate)),
      premium_(std::move(premium)),
      validFrom_(validFrom),
      validTo_(validTo) {
    if (policyNumber_.empty() || vehiclePlate_.empty()) {
        throw TransportException("Insurance requires policy number and vehicle plate");
    }
    if (premium_.isNegative()) {
        throw TransportException("Insurance premium must not be negative");
    }
    if (validTo_.isBefore(validFrom_)) {
        throw TransportException("Insurance validity period is invalid");
    }
}

bool Insurance::isActiveOn(const TransportDate& date) const noexcept {
    return !date.isBefore(validFrom_) && !date.isAfter(validTo_);
}

void Insurance::renew(const TransportDate& newValidTo, const Money& newPremium) {
    if (!newValidTo.isAfter(validFrom_)) {
        throw TransportException("New insurance expiry must be after the start date");
    }
    if (newPremium.isNegative()) {
        throw TransportException("Insurance premium must not be negative");
    }
    validTo_ = newValidTo;
    premium_ = newPremium;
}

const Money& Insurance::annualCost() const noexcept {
    return premium_;
}

} // namespace transport
