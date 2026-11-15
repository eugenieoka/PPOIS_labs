#include "transport/cargo/PerishableCargo.h"

#include "transport/Constants.h"
#include "transport/exceptions/InvalidCargoDataException.h"

#include <algorithm>
#include <utility>

namespace transport {

PerishableCargo::PerishableCargo(int64_t cargoId, std::string description, double weightKg,
                                 double volumeM3, Money declaredValue, TransportDate expiryDate,
                                 double minTempC, double maxTempC)
    : Cargo(cargoId, std::move(description), weightKg, volumeM3, std::move(declaredValue)),
      expiryDate_(expiryDate),
      minTempC_(minTempC),
      maxTempC_(maxTempC) {
    if (minTempC_ >= maxTempC_) {
        throw InvalidCargoDataException("Perishable cargo temperature range is invalid");
    }
}

bool PerishableCargo::isExpiredOn(const TransportDate& date) const noexcept {
    return date.isAfter(expiryDate_);
}

bool PerishableCargo::requiresRefrigeration() const noexcept {
    return minTempC_ <= constants::kRefrigerationThresholdC;
}

int PerishableCargo::remainingDays(const TransportDate& date) const noexcept {
    return std::max(0, date.daysUntil(expiryDate_));
}

Money PerishableCargo::handlingCost() const {
    return Cargo::handlingCost().add(Money(constants::kPerishableHandlingSurcharge, "RUB"));
}

std::string PerishableCargo::typeName() const {
    return "PerishableCargo";
}

} // namespace transport
