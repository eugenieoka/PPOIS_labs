#pragma once

#include <cstdint>
#include <string>

#include "transport/core/Money.h"

namespace transport {

class Cargo {
public:
    Cargo(int64_t cargoId, std::string description, double weightKg,
          double volumeM3, Money declaredValue);
    virtual ~Cargo() = default;

    [[nodiscard]] int64_t cargoId() const noexcept { return cargoId_; }
    [[nodiscard]] const std::string& description() const noexcept { return description_; }
    [[nodiscard]] double weightKg() const noexcept { return weightKg_; }
    [[nodiscard]] double volumeM3() const noexcept { return volumeM3_; }
    [[nodiscard]] const Money& declaredValue() const noexcept { return declaredValue_; }

    [[nodiscard]] double density() const noexcept;
    [[nodiscard]] bool isOverweight(double maxWeightKg) const;
    [[nodiscard]] virtual Money handlingCost() const;
    [[nodiscard]] virtual std::string typeName() const;
    [[nodiscard]] virtual std::string summary() const;

private:
    int64_t cargoId_ = 0;
    std::string description_;
    double weightKg_ = 0.0;
    double volumeM3_ = 0.0;
    Money declaredValue_;
};

} // namespace transport
