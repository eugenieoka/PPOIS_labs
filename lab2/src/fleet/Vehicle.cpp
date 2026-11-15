#include "transport/fleet/Vehicle.h"

#include "transport/Constants.h"
#include "transport/cargo/Cargo.h"
#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

Vehicle::Vehicle(std::string plateNumber, double capacityKg, double capacityM3,
                 double mileageKm, double baseRatePerKm, Machine machine)
    : plateNumber_(std::move(plateNumber)),
      capacityKg_(capacityKg),
      capacityM3_(capacityM3),
      mileageKm_(mileageKm),
      baseRatePerKm_(baseRatePerKm),
      machine_(std::move(machine)) {
    if (plateNumber_.empty()) {
        throw TransportException("Vehicle plate number must not be empty");
    }
    if (capacityKg_ <= 0.0 || capacityM3_ <= 0.0) {
        throw TransportException("Vehicle capacities must be positive");
    }
    if (mileageKm_ < 0.0 || baseRatePerKm_ < 0.0) {
        throw TransportException("Vehicle mileage and rate must not be negative");
    }
}

bool Vehicle::canCarry(const Cargo& cargo) const {
    return status_ == VehicleStatus::Available &&
           cargo.weightKg() <= capacityKg_ && cargo.volumeM3() <= capacityM3_;
}

void Vehicle::addMileage(double distanceKm) {
    if (distanceKm < 0.0) {
        throw TransportException("Mileage increment must not be negative");
    }
    mileageKm_ += distanceKm;
}

void Vehicle::changeStatus(VehicleStatus status) noexcept {
    status_ = status;
}

bool Vehicle::needsMaintenance() const noexcept {
    return mileageKm_ >= constants::kMaintenanceMileageLimitKm;
}

bool Vehicle::isAvailable() const noexcept {
    return status_ == VehicleStatus::Available;
}

Money Vehicle::baseCost(double distanceKm) const {
    if (distanceKm < 0.0) {
        throw TransportException("Distance must not be negative");
    }
    return Money(distanceKm * baseRatePerKm_, "RUB");
}

std::string Vehicle::typeName() const {
    return "Vehicle";
}

} // namespace transport
