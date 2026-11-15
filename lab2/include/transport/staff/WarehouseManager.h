#pragma once

#include <memory>
#include <string>

#include "transport/staff/Manager.h"

namespace transport {

class Cargo;
class StorageUnit;
class Warehouse;

class WarehouseManager : public Manager {
public:
    WarehouseManager(int64_t employeeId, std::string fullName, Address address, std::string phone,
                     Salary salary, TransportDate hireDate, std::string department,
                     Money budget, std::shared_ptr<Warehouse> warehouse);

    [[nodiscard]] const std::shared_ptr<Warehouse>& warehouse() const noexcept { return warehouse_; }

    void receiveCargo(const Cargo& cargo, StorageUnit& unit);
    void releaseCargo(const Cargo& cargo, StorageUnit& unit);
    [[nodiscard]] Money inventoryValue() const;
    [[nodiscard]] std::string capacityReport() const;
    [[nodiscard]] std::string roleName() const override;

private:
    std::shared_ptr<Warehouse> warehouse_;
};

} // namespace transport
