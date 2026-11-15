#pragma once

#include <string>

#include "transport/cargo/Cargo.h"

namespace transport {

class FragileCargo : public Cargo {
public:
    FragileCargo(int64_t cargoId, std::string description, double weightKg,
                 double volumeM3, Money declaredValue, std::string packaging,
                 int maxDropHeightCm);

    [[nodiscard]] const std::string& packaging() const noexcept { return packaging_; }
    [[nodiscard]] int maxDropHeightCm() const noexcept { return maxDropHeightCm_; }

    [[nodiscard]] Money handlingCost() const override;
    [[nodiscard]] bool requiresSpecialPackaging() const noexcept;
    [[nodiscard]] std::string typeName() const override;

private:
    std::string packaging_;
    int maxDropHeightCm_ = 0;
};

} // namespace transport
