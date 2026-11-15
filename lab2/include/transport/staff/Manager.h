#pragma once

#include <string>

#include "transport/core/Money.h"
#include "transport/staff/Employee.h"

namespace transport {

class Manager : public Employee {
public:
    Manager(int64_t employeeId, std::string fullName, Address address, std::string phone,
            Salary salary, TransportDate hireDate, std::string department, Money budget);

    [[nodiscard]] const std::string& department() const noexcept { return department_; }
    [[nodiscard]] const Money& budget() const noexcept { return budget_; }

    [[nodiscard]] Money remainingBudget() const noexcept;
    void approveExpense(const Money& expense);
    void increaseBudget(const Money& amount);
    [[nodiscard]] std::string roleName() const override;

private:
    std::string department_;
    Money budget_;
};

} // namespace transport
