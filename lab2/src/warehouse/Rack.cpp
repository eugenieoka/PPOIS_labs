#include "transport/warehouse/Rack.h"

#include "transport/cargo/Cargo.h"
#include "transport/exceptions/TransportException.h"
#include "transport/exceptions/WarehouseCapacityException.h"

#include <algorithm>

namespace transport {

Rack::Rack(int number, int levels, double capacityKg)
    : number_(number), levels_(levels), capacityKg_(capacityKg) {
    if (number_ <= 0) {
        throw TransportException("Rack number must be positive");
    }
    if (levels_ < 1) {
        throw TransportException("Rack levels must be positive");
    }
    if (capacityKg_ <= 0.0) {
        throw TransportException("Rack capacity must be positive");
    }
}

double Rack::freeCapacity() const noexcept {
    return capacityKg_ - usedCapacityKg_;
}

void Rack::place(const Cargo& cargo) {
    if (cargo.weightKg() > freeCapacity()) {
        throw WarehouseCapacityException("Rack capacity exceeded");
    }
    const auto found = std::find(cargoIds_.begin(), cargoIds_.end(), cargo.cargoId());
    if (found != cargoIds_.end()) {
        throw TransportException("Cargo is already stored on this rack");
    }
    cargoIds_.push_back(cargo.cargoId());
    usedCapacityKg_ += cargo.weightKg();
}

void Rack::take(const Cargo& cargo) {
    const auto found = std::find(cargoIds_.begin(), cargoIds_.end(), cargo.cargoId());
    if (found == cargoIds_.end()) {
        throw TransportException("Cargo is not stored on this rack");
    }
    cargoIds_.erase(found);
    usedCapacityKg_ -= cargo.weightKg();
}

bool Rack::isEmpty() const noexcept {
    return cargoIds_.empty();
}

} // namespace transport
