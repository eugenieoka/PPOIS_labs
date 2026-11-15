#pragma once

#include <cstddef>
#include <string>

#include "transport/Enums.h"
#include "transport/core/Money.h"
#include "transport/fleet/Machine.h"

namespace transport {

class Cargo;

class Vehicle {
public:
    Vehicle(std::string plateNumber, double capacityKg, double capacityM3,
            double mileageKm, double baseRatePerKm, Machine machine);
    virtual ~Vehicle() = default;

    [[nodiscard]] const std::string& plateNumber() const noexcept { return plateNumber_; }
    [[nodiscard]] double capacityKg() const noexcept { return capacityKg_; }
    [[nodiscard]] double capacityM3() const noexcept { return capacityM3_; }
    [[nodiscard]] double mileageKm() const noexcept { return mileageKm_; }
    [[nodiscard]] VehicleStatus status() const noexcept { return status_; }
    [[nodiscard]] double baseRatePerKm() const noexcept { return baseRatePerKm_; }
    [[nodiscard]] const Machine& machine() const noexcept { return machine_; }

    [[nodiscard]] virtual bool canCarry(const Cargo& cargo) const;
    void addMileage(double distanceKm);
    void changeStatus(VehicleStatus status) noexcept;
    [[nodiscard]] bool needsMaintenance() const noexcept;
    [[nodiscard]] bool isAvailable() const noexcept;
    [[nodiscard]] Money baseCost(double distanceKm) const;
    [[nodiscard]] virtual std::string typeName() const;

private:
    std::string plateNumber_;
    double capacityKg_ = 0.0;
    double capacityM3_ = 0.0;
    double mileageKm_ = 0.0;
    VehicleStatus status_ = VehicleStatus::Available;
    double baseRatePerKm_ = 0.0;
    Machine machine_;
};

} // namespace transport
