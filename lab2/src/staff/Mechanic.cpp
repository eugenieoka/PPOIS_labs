#include "transport/staff/Mechanic.h"

#include "transport/exceptions/TransportException.h"
#include "transport/fleet/MaintenanceRecord.h"
#include "transport/fleet/Vehicle.h"

#include <utility>

namespace transport {

Mechanic::Mechanic(int64_t employeeId, std::string fullName, Address address, std::string phone,
                   Salary salary, TransportDate hireDate, std::string specialization)
    : Employee(employeeId, std::move(fullName), std::move(address), std::move(phone),
               std::move(salary), hireDate),
      specialization_(std::move(specialization)) {
    if (specialization_.empty()) {
        throw TransportException("Mechanic specialization must not be empty");
    }
}

bool Mechanic::canRepair(const Vehicle& vehicle) const {
    return specialization_ == "Universal" || specialization_ == vehicle.typeName();
}

MaintenanceRecord Mechanic::performMaintenance(Vehicle& vehicle, const TransportDate& date,
                                               const Money& cost) {
    if (!canRepair(vehicle)) {
        throw TransportException("Mechanic cannot repair this vehicle type");
    }
    vehicle.changeStatus(VehicleStatus::Maintenance);
    ++repairsCompleted_;
    return MaintenanceRecord(vehicle.plateNumber(), employeeId(), date, cost,
                             "Maintenance by " + fullName());
}

void Mechanic::completeRepair(Vehicle& vehicle) {
    vehicle.changeStatus(VehicleStatus::Available);
}

std::string Mechanic::roleName() const {
    return "Mechanic";
}

} // namespace transport
