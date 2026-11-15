#include "transport/cargo/FragileCargo.h"

#include "transport/Constants.h"
#include "transport/exceptions/InvalidCargoDataException.h"

#include <utility>

namespace transport {

FragileCargo::FragileCargo(int64_t cargoId, std::string description, double weightKg,
                           double volumeM3, Money declaredValue, std::string packaging,
                           int maxDropHeightCm)
    : Cargo(cargoId, std::move(description), weightKg, volumeM3, std::move(declaredValue)),
      packaging_(std::move(packaging)),
      maxDropHeightCm_(maxDropHeightCm) {
    if (packaging_.empty()) {
        throw InvalidCargoDataException("Fragile cargo packaging must not be empty");
    }
    if (maxDropHeightCm_ <= 0) {
        throw InvalidCargoDataException("Fragile cargo drop height must be positive");
    }
}

Money FragileCargo::handlingCost() const {
    return Cargo::handlingCost().add(Money(constants::kFragileHandlingSurcharge, "RUB"));
}

bool FragileCargo::requiresSpecialPackaging() const noexcept {
    return true;
}

std::string FragileCargo::typeName() const {
    return "FragileCargo";
}

} // namespace transport
