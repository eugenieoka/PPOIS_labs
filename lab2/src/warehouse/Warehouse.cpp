#include "transport/warehouse/Warehouse.h"

#include "transport/cargo/Cargo.h"
#include "transport/exceptions/TransportException.h"
#include "transport/warehouse/StorageUnit.h"

#include <utility>

namespace transport {

Warehouse::Warehouse(std::string name, Address address)
    : name_(std::move(name)), address_(std::move(address)) {
    if (name_.empty()) {
        throw TransportException("Warehouse name must not be empty");
    }
}

void Warehouse::addZone(const StorageZone& zone) {
    zones_.push_back(zone);
}

void Warehouse::receiveCargo(const Cargo& cargo, StorageUnit& unit) {
    unit.store(cargo);
    inventoryValue_ = inventoryValue_.add(cargo.declaredValue());
}

void Warehouse::shipCargo(const Cargo& cargo, StorageUnit& unit) {
    unit.release(cargo);
    inventoryValue_ = inventoryValue_.subtract(cargo.declaredValue());
}

double Warehouse::totalCapacity() const noexcept {
    double total = 0.0;
    for (const StorageZone& zone : zones_) {
        total += zone.totalCapacity();
    }
    return total;
}

double Warehouse::occupancy() const noexcept {
    double total = 0.0;
    for (const StorageZone& zone : zones_) {
        total += zone.occupancy();
    }
    return total;
}

} // namespace transport
