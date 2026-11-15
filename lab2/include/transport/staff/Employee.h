#pragma once

#include <cstdint>
#include <string>

#include "transport/core/Address.h"
#include "transport/core/Money.h"
#include "transport/core/TransportDate.h"
#include "transport/staff/Salary.h"

namespace transport {

class Employee {
public:
    Employee(int64_t employeeId, std::string fullName, Address address,
             std::string phone, Salary salary, TransportDate hireDate);
    virtual ~Employee() = default;

    [[nodiscard]] int64_t employeeId() const noexcept { return employeeId_; }
    [[nodiscard]] const std::string& fullName() const noexcept { return fullName_; }
    [[nodiscard]] const Address& address() const noexcept { return address_; }
    [[nodiscard]] const std::string& phone() const noexcept { return phone_; }
    [[nodiscard]] const Salary& salary() const noexcept { return salary_; }
    [[nodiscard]] const TransportDate& hireDate() const noexcept { return hireDate_; }
    [[nodiscard]] bool isActive() const noexcept { return active_; }

    [[nodiscard]] Money annualCost() const;
    [[nodiscard]] int tenureYears(const TransportDate& today) const;
    void giveRaise(double percent);
    void deactivate() noexcept;
    void activate() noexcept;
    [[nodiscard]] virtual std::string roleName() const;

private:
    int64_t employeeId_ = 0;
    std::string fullName_;
    Address address_;
    std::string phone_;
    Salary salary_;
    TransportDate hireDate_;
    bool active_ = true;
};

} // namespace transport
