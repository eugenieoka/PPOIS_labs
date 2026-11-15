#pragma once

#include <cstdint>

#include "transport/finance/TaxCalculator.h"
#include "transport/staff/Employee.h"

namespace transport {

class BankAccount;
class Client;
class Invoice;
class Order;
class Payment;

class Accountant : public Employee {
public:
    Accountant(int64_t employeeId, std::string fullName, Address address, std::string phone,
               Salary salary, TransportDate hireDate, TaxCalculator taxCalculator);

    [[nodiscard]] int processedInvoices() const noexcept { return processedInvoices_; }
    [[nodiscard]] const TaxCalculator& taxCalculator() const noexcept { return taxCalculator_; }

    [[nodiscard]] Invoice issueInvoice(const Order& order, const Client& client,
                                       const TransportDate& issuedOn,
                                       const TransportDate& dueOn);
    void processPayment(Payment& payment, BankAccount& account, const TransportDate& date);
    [[nodiscard]] Money calculateSalary(const Employee& employee) const;
    [[nodiscard]] Money taxReport(const Money& income) const;
    [[nodiscard]] std::string roleName() const override;

private:
    int processedInvoices_ = 0;
    TaxCalculator taxCalculator_;
};

} // namespace transport
