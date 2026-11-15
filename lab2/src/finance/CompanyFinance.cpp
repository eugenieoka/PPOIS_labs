#include "transport/finance/CompanyFinance.h"

#include <sstream>
#include <utility>

namespace transport {

CompanyFinance::CompanyFinance(BankAccount account, TaxCalculator taxCalculator)
    : account_(std::move(account)), taxCalculator_(std::move(taxCalculator)) {}

void CompanyFinance::addInvoice(const Invoice& invoice) {
    invoices_.push_back(invoice);
}

void CompanyFinance::recordIncome(const Payment& payment) {
    payments_.push_back(payment);
    if (!payment.isSuccessful()) {
        return;
    }
    account_.deposit(payment.amount());
    profitAnalyzer_.recordIncome(payment.amount());
}

void CompanyFinance::recordExpense(const std::string& category, const Money& amount) {
    account_.withdraw(amount);
    expenseTracker_.addExpense(category, amount);
}

Money CompanyFinance::balance() const {
    return account_.balance();
}

std::vector<Invoice> CompanyFinance::unpaidInvoices() const {
    std::vector<Invoice> result;
    for (const Invoice& invoice : invoices_) {
        if (!invoice.isPaid()) {
            result.push_back(invoice);
        }
    }
    return result;
}

std::string CompanyFinance::monthlyReport() const {
    std::ostringstream out;
    out << "balance=" << account_.balance().format()
        << ", unpaid invoices=" << unpaidInvoices().size()
        << ", gross profit=" << profitAnalyzer_.grossProfit().format()
        << ", margin=" << profitAnalyzer_.margin() << "%";
    return out.str();
}

Money CompanyFinance::taxReport(const Money& income) const {
    return taxCalculator_.netIncome(income);
}

} // namespace transport
