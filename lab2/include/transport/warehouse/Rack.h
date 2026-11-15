#pragma once

#include <cstdint>
#include <vector>

namespace transport {

class Cargo;

class Rack {
public:
    Rack() = default;
    Rack(int number, int levels, double capacityKg);

    [[nodiscard]] int number() const noexcept { return number_; }
    [[nodiscard]] int levels() const noexcept { return levels_; }
    [[nodiscard]] double capacityKg() const noexcept { return capacityKg_; }
    [[nodiscard]] double usedCapacityKg() const noexcept { return usedCapacityKg_; }

    [[nodiscard]] double freeCapacity() const noexcept;
    void place(const Cargo& cargo);
    void take(const Cargo& cargo);
    [[nodiscard]] bool isEmpty() const noexcept;

private:
    int number_ = 0;
    int levels_ = 0;
    double capacityKg_ = 0.0;
    double usedCapacityKg_ = 0.0;
    std::vector<int64_t> cargoIds_;
};

} // namespace transport
