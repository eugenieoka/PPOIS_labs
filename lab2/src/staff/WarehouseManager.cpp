#include "transport/staff/WarehouseManager.h"

#include "transport/cargo/Cargo.h"
#include "transport/exceptions/TransportException.h"
#include "transport/warehouse/StorageUnit.h"
#include "transport/warehouse/Warehouse.h"

#include <sstream>
#include <utility>

namespace transport {

WarehouseManager::WarehouseManager(int64_t employeeId, std::string fullName, Address address,
                                   std::string phone, Salary salary, TransportDate hireDate,
                                   std::string department, Money budget,
                                   std::shared_ptr<Warehouse> warehouse)
    : Manager(employeeId, std::move(fullName), std::move(address), std::move(phone),
              std::move(salary), hireDate, std::move(department), std::move(budget)),
      warehouse_(std::move(warehouse)) {
    if (!warehouse_) {
        throw TransportException("WarehouseManager requires a warehouse");
    }
}

void WarehouseManager::receiveCargo(const Cargo& cargo, StorageUnit& unit) {
    warehouse_->receiveCargo(cargo, unit);
}

void WarehouseManager::releaseCargo(const Cargo& cargo, StorageUnit& unit) {
    warehouse_->shipCargo(cargo, unit);
}

Money WarehouseManager::inventoryValue() const {
    return warehouse_->inventoryValue();
}

std::string WarehouseManager::capacityReport() const {
    std::ostringstream out;
    out << warehouse_->name() << ": capacity=" << warehouse_->totalCapacity()
        << " kg, used=" << warehouse_->occupancy() << " kg";
    return out.str();
}

std::string WarehouseManager::roleName() const {
    return "WarehouseManager";
}

} // namespace transport
