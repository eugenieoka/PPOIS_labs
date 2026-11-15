#pragma once

#include <string>
#include <vector>

#include "transport/core/Address.h"
#include "transport/core/Money.h"
#include "transport/warehouse/StorageZone.h"

namespace transport {

class Cargo;
class StorageUnit;

class Warehouse {
public:
    Warehouse() = default;
    Warehouse(std::string name, Address address);

    [[nodiscard]] const std::string& name() const noexcept { return name_; }
    [[nodiscard]] const Address& address() const noexcept { return address_; }
    [[nodiscard]] const std::vector<StorageZone>& zones() const noexcept { return zones_; }

    void addZone(const StorageZone& zone);
    void receiveCargo(const Cargo& cargo, StorageUnit& unit);
    void shipCargo(const Cargo& cargo, StorageUnit& unit);
    [[nodiscard]] double totalCapacity() const noexcept;
    [[nodiscard]] double occupancy() const noexcept;
    [[nodiscard]] const Money& inventoryValue() const noexcept { return inventoryValue_; }

private:
    std::string name_;
    Address address_;
    std::vector<StorageZone> zones_;
    Money inventoryValue_{0.0, "RUB"};
};

} // namespace transport
