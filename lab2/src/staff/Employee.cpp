#include "transport/staff/Employee.h"

#include "transport/Constants.h"
#include "transport/exceptions/TransportException.h"

#include <cmath>
#include <utility>

namespace transport {
namespace {

constexpr double kMaxRaisePercent = 1000.0;
constexpr double kHundredPercent = 100.0;

} // namespace

Employee::Employee(int64_t employeeId, std::string fullName, Address address,
                   std::string phone, Salary salary, TransportDate hireDate)
    : employeeId_(employeeId),
      fullName_(std::move(fullName)),
      address_(std::move(address)),
      phone_(std::move(phone)),
      salary_(std::move(salary)),
      hireDate_(hireDate) {
    if (employeeId_ <= 0) {
        throw TransportException("Employee id must be positive");
    }
    if (fullName_.empty() || phone_.empty()) {
        throw TransportException("Employee name and phone must not be empty");
    }
}

Money Employee::annualCost() const {
    return salary_.net().multiply(12.0);
}

int Employee::tenureYears(const TransportDate& today) const {
    const int days = hireDate_.daysUntil(today);
    if (days < 0) {
        return 0;
    }
    return days / constants::kDaysPerYear;
}

void Employee::giveRaise(double percent) {
    if (!std::isfinite(percent) || percent <= -kHundredPercent || percent > kMaxRaisePercent) {
        throw TransportException("Raise percent is out of range");
    }
    const Money updatedBase = salary_.base().multiply(1.0 + percent / kHundredPercent);
    salary_ = Salary(updatedBase, salary_.bonus(), salary_.deductions());
}

void Employee::deactivate() noexcept {
    active_ = false;
}

void Employee::activate() noexcept {
    active_ = true;
}

std::string Employee::roleName() const {
    return "Employee";
}

} // namespace transport
