#pragma once

#include <string>
#include <vector>

#include "transport/finance/BankAccount.h"
#include "transport/finance/ExpenseTracker.h"
#include "transport/finance/Invoice.h"
#include "transport/finance/Payment.h"
#include "transport/finance/ProfitAnalyzer.h"
#include "transport/finance/TaxCalculator.h"

namespace transport {

class CompanyFinance {
public:
    CompanyFinance() = default;
    CompanyFinance(BankAccount account, TaxCalculator taxCalculator);

    [[nodiscard]] const BankAccount& account() const noexcept { return account_; }
    [[nodiscard]] const std::vector<Invoice>& invoices() const noexcept { return invoices_; }
    [[nodiscard]] const std::vector<Payment>& payments() const noexcept { return payments_; }

    void addInvoice(const Invoice& invoice);
    void recordIncome(const Payment& payment);
    void recordExpense(const std::string& category, const Money& amount);
    [[nodiscard]] Money balance() const;
    [[nodiscard]] std::vector<Invoice> unpaidInvoices() const;
    [[nodiscard]] std::string monthlyReport() const;
    [[nodiscard]] Money taxReport(const Money& income) const;

private:
    BankAccount account_;
    std::vector<Invoice> invoices_;
    std::vector<Payment> payments_;
    ExpenseTracker expenseTracker_;
    ProfitAnalyzer profitAnalyzer_;
    TaxCalculator taxCalculator_;
};

} // namespace transport
