#include "transport/fleet/Van.h"

#include "transport/Constants.h"
#include "transport/cargo/PerishableCargo.h"
#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

Van::Van(std::string plateNumber, double capacityKg, double capacityM3,
         double mileageKm, double baseRatePerKm, Machine machine,
         bool refrigerated, int slidingDoors)
    : Vehicle(std::move(plateNumber), capacityKg, capacityM3, mileageKm,
              baseRatePerKm, std::move(machine)),
      refrigerated_(refrigerated),
      slidingDoors_(slidingDoors) {
    if (slidingDoors_ < 0) {
        throw TransportException("Van sliding doors count must not be negative");
    }
}

bool Van::canCarryPerishable(const PerishableCargo& cargo) const {
    return refrigerated_ && cargo.requiresRefrigeration();
}

Money Van::loadingCost() const {
    return Money(constants::kVehicleLoadingCost, "RUB");
}

std::string Van::typeName() const {
    return "Van";
}

} // namespace transport
