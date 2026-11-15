#pragma once

#include <string>

#include "transport/staff/DrivingLicense.h"
#include "transport/staff/Employee.h"

namespace transport {

class Driver : public Employee {
public:
    Driver(int64_t employeeId, std::string fullName, Address address, std::string phone,
           Salary salary, TransportDate hireDate, DrivingLicense license, int experienceYears);

    [[nodiscard]] const DrivingLicense& license() const noexcept { return license_; }
    [[nodiscard]] int experienceYears() const noexcept { return experienceYears_; }
    [[nodiscard]] int completedDeliveries() const noexcept { return completedDeliveries_; }
    [[nodiscard]] double rating() const noexcept { return rating_; }
    [[nodiscard]] bool isAvailable() const noexcept { return available_; }

    [[nodiscard]] bool canDrive(const std::string& category) const noexcept;
    void assign();
    void completeDelivery();
    void updateRating(double rating);
    [[nodiscard]] std::string roleName() const override;

private:
    DrivingLicense license_;
    int experienceYears_ = 0;
    int completedDeliveries_ = 0;
    double rating_ = 5.0;
    bool available_ = true;
};

} // namespace transport
