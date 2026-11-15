#include "transport/fleet/Machine.h"

#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

Machine::Machine(std::string serialNumber, std::string fuelType, double fuelPerHour)
    : serialNumber_(std::move(serialNumber)),
      fuelType_(std::move(fuelType)),
      fuelPerHour_(fuelPerHour) {
    if (serialNumber_.empty() || fuelType_.empty()) {
        throw TransportException("Machine serial and fuel type must not be empty");
    }
    if (fuelPerHour_ <= 0.0) {
        throw TransportException("Machine fuel consumption must be positive");
    }
}

void Machine::start() {
    if (running_) {
        throw TransportException("Machine is already running");
    }
    running_ = true;
}

void Machine::stop() {
    if (!running_) {
        throw TransportException("Machine is not running");
    }
    running_ = false;
}

double Machine::consumeFuel(double hours) const {
    if (hours < 0.0) {
        throw TransportException("Machine operating hours must not be negative");
    }
    return hours * fuelPerHour_;
}

} // namespace transport
