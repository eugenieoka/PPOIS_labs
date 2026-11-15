#include "transport/staff/Dispatcher.h"

#include "transport/exceptions/InvalidOrderStateException.h"
#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

Dispatcher::Dispatcher(int64_t employeeId, std::string fullName, Address address, std::string phone,
                       Salary salary, TransportDate hireDate, std::string region,
                       int maxOrders, std::string shift)
    : Employee(employeeId, std::move(fullName), std::move(address), std::move(phone),
               std::move(salary), hireDate),
      region_(std::move(region)),
      maxOrders_(maxOrders),
      shift_(std::move(shift)) {
    if (region_.empty() || shift_.empty()) {
        throw TransportException("Dispatcher region and shift must not be empty");
    }
    if (maxOrders_ <= 0) {
        throw TransportException("Dispatcher max orders must be positive");
    }
}

bool Dispatcher::canHandleMore() const noexcept {
    return activeOrders_ < maxOrders_;
}

void Dispatcher::assignOrder() {
    if (!canHandleMore()) {
        throw TransportException("Dispatcher cannot handle more orders");
    }
    ++activeOrders_;
}

void Dispatcher::releaseOrder() {
    if (activeOrders_ == 0) {
        throw InvalidOrderStateException("Dispatcher has no active orders");
    }
    --activeOrders_;
}

void Dispatcher::closeShift() {
    if (activeOrders_ != 0) {
        throw InvalidOrderStateException("Cannot close a shift with active orders");
    }
    shift_ = "Closed";
}

std::string Dispatcher::roleName() const {
    return "Dispatcher";
}

} // namespace transport
