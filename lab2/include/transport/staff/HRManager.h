#pragma once

#include "transport/staff/Manager.h"

namespace transport {

class HRManager : public Manager {
public:
    HRManager(int64_t employeeId, std::string fullName, Address address, std::string phone,
              Salary salary, TransportDate hireDate, std::string department,
              Money budget, int openPositions);

    [[nodiscard]] int hiredCount() const noexcept { return hiredCount_; }
    [[nodiscard]] int openPositions() const noexcept { return openPositions_; }

    void hire(Employee& employee);
    void fire(Employee& employee);
    void openPosition();
    void closePosition();
    [[nodiscard]] std::string roleName() const override;

private:
    int hiredCount_ = 0;
    int openPositions_ = 0;
};

} // namespace transport
