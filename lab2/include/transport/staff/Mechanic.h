#pragma once

#include <string>

#include "transport/staff/Employee.h"

namespace transport {

class MaintenanceRecord;
class Vehicle;

class Mechanic : public Employee {
public:
    Mechanic(int64_t employeeId, std::string fullName, Address address, std::string phone,
             Salary salary, TransportDate hireDate, std::string specialization);

    [[nodiscard]] const std::string& specialization() const noexcept { return specialization_; }
    [[nodiscard]] int repairsCompleted() const noexcept { return repairsCompleted_; }

    [[nodiscard]] bool canRepair(const Vehicle& vehicle) const;
    [[nodiscard]] MaintenanceRecord performMaintenance(Vehicle& vehicle,
                                                       const TransportDate& date,
                                                       const Money& cost);
    void completeRepair(Vehicle& vehicle);
    [[nodiscard]] std::string roleName() const override;

private:
    std::string specialization_;
    int repairsCompleted_ = 0;
};

} // namespace transport
