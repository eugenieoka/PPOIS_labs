#pragma once

#include <string>

#include "transport/staff/Employee.h"

namespace transport {

class Dispatcher : public Employee {
public:
    Dispatcher(int64_t employeeId, std::string fullName, Address address, std::string phone,
               Salary salary, TransportDate hireDate, std::string region,
               int maxOrders, std::string shift);

    [[nodiscard]] const std::string& region() const noexcept { return region_; }
    [[nodiscard]] int activeOrders() const noexcept { return activeOrders_; }
    [[nodiscard]] int maxOrders() const noexcept { return maxOrders_; }
    [[nodiscard]] const std::string& shift() const noexcept { return shift_; }

    [[nodiscard]] bool canHandleMore() const noexcept;
    void assignOrder();
    void releaseOrder();
    void closeShift();
    [[nodiscard]] std::string roleName() const override;

private:
    std::string region_;
    int activeOrders_ = 0;
    int maxOrders_ = 0;
    std::string shift_;
};

} // namespace transport
