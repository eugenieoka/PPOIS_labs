#pragma once

#include <string>

#include "transport/fleet/Vehicle.h"

namespace transport {

class PerishableCargo;

class Van : public Vehicle {
public:
    Van(std::string plateNumber, double capacityKg, double capacityM3,
        double mileageKm, double baseRatePerKm, Machine machine,
        bool refrigerated, int slidingDoors);

    [[nodiscard]] bool refrigerated() const noexcept { return refrigerated_; }
    [[nodiscard]] int slidingDoors() const noexcept { return slidingDoors_; }

    [[nodiscard]] bool canCarryPerishable(const PerishableCargo& cargo) const;
    [[nodiscard]] Money loadingCost() const;
    [[nodiscard]] std::string typeName() const override;

private:
    bool refrigerated_ = false;
    int slidingDoors_ = 0;
};

} // namespace transport
