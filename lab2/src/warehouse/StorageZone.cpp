#include "transport/warehouse/StorageZone.h"

#include "transport/cargo/Cargo.h"
#include "transport/cargo/PerishableCargo.h"
#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

StorageZone::StorageZone(std::string name, double temperatureC)
    : name_(std::move(name)), temperatureC_(temperatureC) {
    if (name_.empty()) {
        throw TransportException("Storage zone name must not be empty");
    }
}

void StorageZone::addRack(const Rack& rack) {
    racks_.push_back(rack);
}

Rack* StorageZone::findFreeRack(const Cargo& cargo) {
    for (Rack& rack : racks_) {
        if (rack.freeCapacity() >= cargo.weightKg()) {
            return &rack;
        }
    }
    return nullptr;
}

double StorageZone::totalCapacity() const noexcept {
    double total = 0.0;
    for (const Rack& rack : racks_) {
        total += rack.capacityKg();
    }
    return total;
}

double StorageZone::occupancy() const noexcept {
    double total = 0.0;
    for (const Rack& rack : racks_) {
        total += rack.usedCapacityKg();
    }
    return total;
}

bool StorageZone::accepts(const PerishableCargo& cargo) const noexcept {
    return temperatureC_ >= cargo.minTempC() && temperatureC_ <= cargo.maxTempC();
}

} // namespace transport
