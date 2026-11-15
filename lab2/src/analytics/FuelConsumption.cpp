#include "transport/analytics/FuelConsumption.h"

#include "transport/exceptions/TransportException.h"

namespace transport {
namespace {

constexpr double kHundredKm = 100.0;

} // namespace

FuelConsumption::FuelConsumption(double distanceKm, double litersUsed, double pricePerLiter)
    : distanceKm_(distanceKm), litersUsed_(litersUsed), pricePerLiter_(pricePerLiter) {
    if (distanceKm_ <= 0.0 || litersUsed_ < 0.0 || pricePerLiter_ < 0.0) {
        throw TransportException("Fuel consumption values are invalid");
    }
}

double FuelConsumption::per100Km() const {
    return litersUsed_ / distanceKm_ * kHundredKm;
}

Money FuelConsumption::totalCost() const {
    return Money(litersUsed_ * pricePerLiter_, "RUB");
}

bool FuelConsumption::isEfficient(double thresholdPer100Km) const {
    if (thresholdPer100Km <= 0.0) {
        throw TransportException("Efficiency threshold must be positive");
    }
    return per100Km() <= thresholdPer100Km;
}

} // namespace transport
