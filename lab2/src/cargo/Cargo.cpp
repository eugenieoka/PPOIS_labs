#include "transport/cargo/Cargo.h"

#include "transport/Constants.h"
#include "transport/exceptions/InvalidCargoDataException.h"

#include <sstream>
#include <utility>

namespace transport {

Cargo::Cargo(int64_t cargoId, std::string description, double weightKg,
             double volumeM3, Money declaredValue)
    : cargoId_(cargoId),
      description_(std::move(description)),
      weightKg_(weightKg),
      volumeM3_(volumeM3),
      declaredValue_(std::move(declaredValue)) {
    if (cargoId_ <= 0) {
        throw InvalidCargoDataException("Cargo id must be positive");
    }
    if (description_.empty()) {
        throw InvalidCargoDataException("Cargo description must not be empty");
    }
    if (weightKg_ <= 0.0 || volumeM3_ <= 0.0) {
        throw InvalidCargoDataException("Cargo weight and volume must be positive");
    }
    if (declaredValue_.isNegative()) {
        throw InvalidCargoDataException("Cargo declared value must not be negative");
    }
}

double Cargo::density() const noexcept {
    return weightKg_ / volumeM3_;
}

bool Cargo::isOverweight(double maxWeightKg) const {
    if (maxWeightKg <= 0.0) {
        throw InvalidCargoDataException("Maximum weight must be positive");
    }
    return weightKg_ > maxWeightKg;
}

Money Cargo::handlingCost() const {
    return Money(weightKg_ * constants::kHandlingRatePerKg, "RUB");
}

std::string Cargo::typeName() const {
    return "Cargo";
}

std::string Cargo::summary() const {
    std::ostringstream out;
    out << typeName() << " #" << cargoId_ << " " << description_
        << " (" << weightKg_ << " kg, " << volumeM3_ << " m3)";
    return out.str();
}

} // namespace transport
