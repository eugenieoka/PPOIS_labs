#include "transport/fleet/MaintenanceRecord.h"

#include "transport/exceptions/TransportException.h"

#include <sstream>
#include <utility>

namespace transport {

MaintenanceRecord::MaintenanceRecord(std::string vehiclePlate, int64_t mechanicId,
                                     TransportDate performedOn, Money cost,
                                     std::string description)
    : vehiclePlate_(std::move(vehiclePlate)),
      mechanicId_(mechanicId),
      performedOn_(performedOn),
      cost_(std::move(cost)),
      description_(std::move(description)) {
    if (vehiclePlate_.empty() || description_.empty()) {
        throw TransportException("Maintenance record requires vehicle plate and description");
    }
    if (mechanicId_ <= 0) {
        throw TransportException("Maintenance record mechanic id must be positive");
    }
    if (cost_.isNegative()) {
        throw TransportException("Maintenance cost must not be negative");
    }
}

Money MaintenanceRecord::totalCostWithTax(double taxRate) const {
    if (taxRate < 0.0 || taxRate > 1.0) {
        throw TransportException("Tax rate must be between 0 and 1");
    }
    return cost_.multiply(1.0 + taxRate);
}

bool MaintenanceRecord::isRecent(const TransportDate& today, int days) const {
    if (days <= 0) {
        throw TransportException("Recent days interval must be positive");
    }
    const int elapsed = performedOn_.daysUntil(today);
    return elapsed >= 0 && elapsed <= days;
}

std::string MaintenanceRecord::summary() const {
    std::ostringstream out;
    out << vehiclePlate_ << ": " << description_ << " (" << cost_.format() << ")";
    return out.str();
}

} // namespace transport
