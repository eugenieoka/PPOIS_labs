#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace transport {

class Cargo;

class StorageUnit {
public:
    StorageUnit() = default;
    StorageUnit(std::string unitId, double capacityKg, std::string zoneName);

    [[nodiscard]] const std::string& unitId() const noexcept { return unitId_; }
    [[nodiscard]] double capacityKg() const noexcept { return capacityKg_; }
    [[nodiscard]] const std::string& zoneName() const noexcept { return zoneName_; }
    [[nodiscard]] double usedCapacityKg() const noexcept { return usedCapacityKg_; }

    [[nodiscard]] bool canStore(const Cargo& cargo) const noexcept;
    void store(const Cargo& cargo);
    void release(const Cargo& cargo);
    [[nodiscard]] double occupancy() const noexcept;

private:
    std::string unitId_;
    double capacityKg_ = 0.0;
    std::string zoneName_;
    double usedCapacityKg_ = 0.0;
    std::vector<int64_t> cargoIds_;
};

} // namespace transport
