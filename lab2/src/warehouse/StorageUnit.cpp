#include "transport/warehouse/StorageUnit.h"

#include "transport/cargo/Cargo.h"
#include "transport/exceptions/TransportException.h"
#include "transport/exceptions/WarehouseCapacityException.h"

#include <algorithm>
#include <utility>

namespace transport {

StorageUnit::StorageUnit(std::string unitId, double capacityKg, std::string zoneName)
    : unitId_(std::move(unitId)), capacityKg_(capacityKg), zoneName_(std::move(zoneName)) {
    if (unitId_.empty() || zoneName_.empty()) {
        throw TransportException("Storage unit id and zone must not be empty");
    }
    if (capacityKg_ <= 0.0) {
        throw TransportException("Storage unit capacity must be positive");
    }
}

bool StorageUnit::canStore(const Cargo& cargo) const noexcept {
    return usedCapacityKg_ + cargo.weightKg() <= capacityKg_;
}

void StorageUnit::store(const Cargo& cargo) {
    if (!canStore(cargo)) {
        throw WarehouseCapacityException("Storage unit capacity exceeded");
    }
    const auto found = std::find(cargoIds_.begin(), cargoIds_.end(), cargo.cargoId());
    if (found != cargoIds_.end()) {
        throw TransportException("Cargo is already stored in this unit");
    }
    cargoIds_.push_back(cargo.cargoId());
    usedCapacityKg_ += cargo.weightKg();
}

void StorageUnit::release(const Cargo& cargo) {
    const auto found = std::find(cargoIds_.begin(), cargoIds_.end(), cargo.cargoId());
    if (found == cargoIds_.end()) {
        throw TransportException("Cargo is not stored in this unit");
    }
    cargoIds_.erase(found);
    usedCapacityKg_ -= cargo.weightKg();
}

double StorageUnit::occupancy() const noexcept {
    return usedCapacityKg_;
}

} // namespace transport
