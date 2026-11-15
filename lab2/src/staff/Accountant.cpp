#include "transport/staff/Accountant.h"

#include "transport/finance/BankAccount.h"
#include "transport/finance/Invoice.h"
#include "transport/finance/Payment.h"
#include "transport/orders/Client.h"
#include "transport/orders/Order.h"

#include <string>
#include <utility>

namespace transport {

Accountant::Accountant(int64_t employeeId, std::string fullName, Address address, std::string phone,
                       Salary salary, TransportDate hireDate, TaxCalculator taxCalculator)
    : Employee(employeeId, std::move(fullName), std::move(address), std::move(phone),
               std::move(salary), hireDate),
      taxCalculator_(std::move(taxCalculator)) {}

Invoice Accountant::issueInvoice(const Order& order, const Client& client,
                                 const TransportDate& issuedOn,
                                 const TransportDate& dueOn) {
    ++processedInvoices_;
    const std::string number = "INV-" + std::to_string(processedInvoices_);
    return Invoice(number, order.orderId(), client.clientId(), order.price(), issuedOn, dueOn);
}

void Accountant::processPayment(Payment& payment, BankAccount& account, const TransportDate& date) {
    payment.process(date);
    if (payment.isSuccessful()) {
        account.deposit(payment.amount());
    }
}

Money Accountant::calculateSalary(const Employee& employee) const {
    return employee.salary().net();
}

Money Accountant::taxReport(const Money& income) const {
    return taxCalculator_.incomeTax(income);
}

std::string Accountant::roleName() const {
    return "Accountant";
}

} // namespace transport
