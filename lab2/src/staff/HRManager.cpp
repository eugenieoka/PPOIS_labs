#include "transport/staff/HRManager.h"

#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

HRManager::HRManager(int64_t employeeId, std::string fullName, Address address, std::string phone,
                     Salary salary, TransportDate hireDate, std::string department,
                     Money budget, int openPositions)
    : Manager(employeeId, std::move(fullName), std::move(address), std::move(phone),
              std::move(salary), hireDate, std::move(department), std::move(budget)),
      openPositions_(openPositions) {
    if (openPositions_ < 0) {
        throw TransportException("Open positions must not be negative");
    }
}

void HRManager::hire(Employee& employee) {
    if (openPositions_ <= 0) {
        throw TransportException("No open positions available");
    }
    if (employee.isActive()) {
        throw TransportException("Employee is already active");
    }
    employee.activate();
    ++hiredCount_;
    --openPositions_;
}

void HRManager::fire(Employee& employee) {
    if (!employee.isActive()) {
        throw TransportException("Employee is already inactive");
    }
    employee.deactivate();
    ++openPositions_;
}

void HRManager::openPosition() {
    ++openPositions_;
}

void HRManager::closePosition() {
    if (openPositions_ <= 0) {
        throw TransportException("No open positions to close");
    }
    --openPositions_;
}

std::string HRManager::roleName() const {
    return "HRManager";
}

} // namespace transport
