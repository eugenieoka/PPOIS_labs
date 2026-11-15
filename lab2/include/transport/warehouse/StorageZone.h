#pragma once

#include <string>
#include <vector>

#include "transport/warehouse/Rack.h"

namespace transport {

class Cargo;
class PerishableCargo;

class StorageZone {
public:
    StorageZone() = default;
    StorageZone(std::string name, double temperatureC);

    [[nodiscard]] const std::string& name() const noexcept { return name_; }
    [[nodiscard]] double temperatureC() const noexcept { return temperatureC_; }
    [[nodiscard]] const std::vector<Rack>& racks() const noexcept { return racks_; }

    void addRack(const Rack& rack);
    [[nodiscard]] Rack* findFreeRack(const Cargo& cargo);
    [[nodiscard]] double totalCapacity() const noexcept;
    [[nodiscard]] double occupancy() const noexcept;
    [[nodiscard]] bool accepts(const PerishableCargo& cargo) const noexcept;

private:
    std::string name_;
    double temperatureC_ = 0.0;
    std::vector<Rack> racks_;
};

} // namespace transport
