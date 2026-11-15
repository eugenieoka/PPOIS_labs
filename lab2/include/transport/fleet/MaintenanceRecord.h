#pragma once

#include <cstdint>
#include <string>

#include "transport/core/Money.h"
#include "transport/core/TransportDate.h"

namespace transport {

class MaintenanceRecord {
public:
    MaintenanceRecord() = default;
    MaintenanceRecord(std::string vehiclePlate, int64_t mechanicId,
                      TransportDate performedOn, Money cost, std::string description);

    [[nodiscard]] const std::string& vehiclePlate() const noexcept { return vehiclePlate_; }
    [[nodiscard]] int64_t mechanicId() const noexcept { return mechanicId_; }
    [[nodiscard]] const TransportDate& performedOn() const noexcept { return performedOn_; }
    [[nodiscard]] const Money& cost() const noexcept { return cost_; }
    [[nodiscard]] const std::string& description() const noexcept { return description_; }

    [[nodiscard]] Money totalCostWithTax(double taxRate) const;
    [[nodiscard]] bool isRecent(const TransportDate& today, int days) const;
    [[nodiscard]] std::string summary() const;

private:
    std::string vehiclePlate_;
    int64_t mechanicId_ = 0;
    TransportDate performedOn_;
    Money cost_;
    std::string description_;
};

} // namespace transport
