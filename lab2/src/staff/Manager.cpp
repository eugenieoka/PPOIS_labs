#include "transport/staff/Manager.h"

#include "transport/exceptions/InsufficientFundsException.h"
#include "transport/exceptions/InvalidMoneyException.h"
#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

Manager::Manager(int64_t employeeId, std::string fullName, Address address, std::string phone,
                 Salary salary, TransportDate hireDate, std::string department, Money budget)
    : Employee(employeeId, std::move(fullName), std::move(address), std::move(phone),
               std::move(salary), hireDate),
      department_(std::move(department)),
      budget_(std::move(budget)) {
    if (department_.empty()) {
        throw TransportException("Manager department must not be empty");
    }
    if (budget_.isNegative()) {
        throw InvalidMoneyException("Manager budget must not be negative");
    }
}

const Money& Manager::remainingBudget() const noexcept {
    return budget_;
}

void Manager::approveExpense(const Money& expense) {
    if (expense.isNegative()) {
        throw InvalidMoneyException("Expense must not be negative");
    }
    if (expense.amount() > budget_.amount()) {
        throw InsufficientFundsException("Manager budget is insufficient");
    }
    budget_ = budget_.subtract(expense);
}

void Manager::increaseBudget(const Money& amount) {
    if (amount.isNegative()) {
        throw InvalidMoneyException("Budget increase must not be negative");
    }
    budget_ = budget_.add(amount);
}

std::string Manager::roleName() const {
    return "Manager";
}

} // namespace transport
