#pragma once

#include "transport/core/Money.h"

namespace transport {

class FuelConsumption {
public:
    FuelConsumption() = default;
    FuelConsumption(double distanceKm, double litersUsed, double pricePerLiter);

    [[nodiscard]] double distanceKm() const noexcept { return distanceKm_; }
    [[nodiscard]] double litersUsed() const noexcept { return litersUsed_; }
    [[nodiscard]] double pricePerLiter() const noexcept { return pricePerLiter_; }

    [[nodiscard]] double per100Km() const;
    [[nodiscard]] Money totalCost() const;
    [[nodiscard]] bool isEfficient(double thresholdPer100Km) const;

private:
    double distanceKm_ = 0.0;
    double litersUsed_ = 0.0;
    double pricePerLiter_ = 0.0;
};

} // namespace transport
