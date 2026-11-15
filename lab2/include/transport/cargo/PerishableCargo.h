#pragma once

#include "transport/cargo/Cargo.h"
#include "transport/core/TransportDate.h"

namespace transport {

class PerishableCargo : public Cargo {
public:
    PerishableCargo(int64_t cargoId, std::string description, double weightKg,
                    double volumeM3, Money declaredValue, TransportDate expiryDate,
                    double minTempC, double maxTempC);

    [[nodiscard]] const TransportDate& expiryDate() const noexcept { return expiryDate_; }
    [[nodiscard]] double minTempC() const noexcept { return minTempC_; }
    [[nodiscard]] double maxTempC() const noexcept { return maxTempC_; }

    [[nodiscard]] bool isExpiredOn(const TransportDate& date) const noexcept;
    [[nodiscard]] bool requiresRefrigeration() const noexcept;
    [[nodiscard]] int remainingDays(const TransportDate& date) const noexcept;
    [[nodiscard]] Money handlingCost() const override;
    [[nodiscard]] std::string typeName() const override;

private:
    TransportDate expiryDate_;
    double minTempC_ = 0.0;
    double maxTempC_ = 0.0;
};

} // namespace transport
