#include "transport/fleet/Truck.h"

#include "transport/Constants.h"
#include "transport/exceptions/TransportException.h"
#include "transport/fleet/Trailer.h"

#include <utility>

namespace transport {

Truck::Truck(std::string plateNumber, double capacityKg, double capacityM3,
             double mileageKm, double baseRatePerKm, Machine machine, int axles)
    : Vehicle(std::move(plateNumber), capacityKg, capacityM3, mileageKm,
              baseRatePerKm, std::move(machine)),
      axles_(axles) {
    if (axles_ < constants::kMinimumTruckAxles) {
        throw TransportException("Truck must have at least two axles");
    }
}

double Truck::maxPayload() const noexcept {
    return axles_ >= 3
        ? capacityKg() * constants::kThreeAxleCapacityMultiplier
        : capacityKg();
}

void Truck::attachTrailer(std::shared_ptr<Trailer> trailer) {
    if (!trailer) {
        throw TransportException("Trailer must not be null");
    }
    if (!trailer->isAvailable()) {
        throw TransportException("Trailer is already attached");
    }
    trailer->attach();
    trailer_ = std::move(trailer);
}

void Truck::detachTrailer() {
    if (!trailer_) {
        throw TransportException("Truck has no trailer");
    }
    trailer_->detach();
    trailer_.reset();
}

std::string Truck::typeName() const {
    return "Truck";
}

} // namespace transport
