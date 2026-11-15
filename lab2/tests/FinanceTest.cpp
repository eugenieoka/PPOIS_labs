#include "TestHelpers.h"

#include <gtest/gtest.h>

#include <limits>

#include "transport/exceptions/InsufficientFundsException.h"
#include "transport/exceptions/InvalidMoneyException.h"
#include "transport/exceptions/PaymentException.h"
#include "transport/exceptions/TransportException.h"
#include "transport/finance/BankAccount.h"
#include "transport/finance/CompanyFinance.h"
#include "transport/finance/ExpenseTracker.h"
#include "transport/finance/Invoice.h"
#include "transport/finance/Payment.h"
#include "transport/finance/ProfitAnalyzer.h"
#include "transport/finance/TaxCalculator.h"

namespace {

using namespace transport;
using namespace transport::test;

TEST(BankAccountTest, DepositsWithdrawsAndTransfers) {
    BankAccount first = bankAccount(1000.0);
    BankAccount second = bankAccount(0.0);

    first.deposit(Money(500.0, "RUB"));
    EXPECT_NEAR(first.balance().amount(), 1500.0, 1e-9);
    first.withdraw(Money(200.0, "RUB"));
    EXPECT_NEAR(first.balance().amount(), 1300.0, 1e-9);
    EXPECT_TRUE(first.canWithdraw(Money(1300.0, "RUB")));
    EXPECT_FALSE(first.canWithdraw(Money(1301.0, "RUB")));

    first.transferTo(second, Money(300.0, "RUB"));
    EXPECT_NEAR(first.balance().amount(), 1000.0, 1e-9);
    EXPECT_NEAR(second.balance().amount(), 300.0, 1e-9);
}

TEST(BankAccountTest, RejectsInvalidOperations) {
    EXPECT_THROW(BankAccount("", Money(1.0, "RUB")), TransportException);
    EXPECT_THROW(BankAccount("IBAN", Money(-1.0, "RUB")), InvalidMoneyException);

    BankAccount account = bankAccount(100.0);
    EXPECT_THROW(account.deposit(Money(-1.0, "RUB")), InvalidMoneyException);
    EXPECT_THROW(account.withdraw(Money(-1.0, "RUB")), InvalidMoneyException);
    EXPECT_THROW(account.withdraw(Money(200.0, "RUB")), InsufficientFundsException);
    EXPECT_THROW((void)account.canWithdraw(Money(-1.0, "RUB")), InvalidMoneyException);
    EXPECT_THROW(account.transferTo(account, Money(1.0, "RUB")), TransportException);
}

TEST(InvoiceTest, PaysAndComputesVat) {
    Invoice invoice("INV-1", 1, 1, Money(1000.0, "RUB"),
                    date(1, 1, 2026), date(10, 1, 2026));
    EXPECT_FALSE(invoice.isPaid());
    EXPECT_NEAR(invoice.remaining().amount(), 1000.0, 1e-9);
    EXPECT_FALSE(invoice.isOverdue(date(5, 1, 2026)));
    EXPECT_TRUE(invoice.isOverdue(date(11, 1, 2026)));
    EXPECT_NEAR(invoice.totalWithVat(0.2).amount(), 1200.0, 1e-9);

    invoice.pay(Money(1000.0, "RUB"));
    EXPECT_TRUE(invoice.isPaid());
    EXPECT_NEAR(invoice.remaining().amount(), 0.0, 1e-9);
    EXPECT_FALSE(invoice.isOverdue(date(11, 1, 2026)));
    EXPECT_THROW(invoice.pay(Money(1.0, "RUB")), PaymentException);

    Invoice marked("INV-2", 1, 1, Money(500.0, "RUB"), date(), date(10, 1, 2026));
    marked.markPaid();
    EXPECT_TRUE(marked.isPaid());
    EXPECT_NEAR(marked.remaining().amount(), 0.0, 1e-9);
}

TEST(InvoiceTest, RejectsInvalidData) {
    EXPECT_THROW(Invoice("", 1, 1, Money(1.0, "RUB"), date(), date()), TransportException);
    EXPECT_THROW(Invoice("I", 0, 1, Money(1.0, "RUB"), date(), date()), TransportException);
    EXPECT_THROW(Invoice("I", 1, 0, Money(1.0, "RUB"), date(), date()), TransportException);
    EXPECT_THROW(Invoice("I", 1, 1, Money(-1.0, "RUB"), date(), date()),
                 InvalidMoneyException);
    EXPECT_THROW(Invoice("I", 1, 1, Money(1.0, "RUB"), date(2, 1, 2026), date()),
                 TransportException);

    Invoice invoice("I", 1, 1, Money(100.0, "RUB"), date(), date(10, 1, 2026));
    EXPECT_THROW(invoice.totalWithVat(-0.1), PaymentException);
    EXPECT_THROW(invoice.totalWithVat(1.1), PaymentException);
    EXPECT_THROW(invoice.pay(Money(-1.0, "RUB")), PaymentException);
    EXPECT_THROW(invoice.pay(Money(50.0, "RUB")), PaymentException);
}

TEST(PaymentTest, ProcessesAndRefunds) {
    Payment payment("INV-1", Money(1000.0, "RUB"), "Card");
    EXPECT_FALSE(payment.isSuccessful());
    payment.process(date());
    EXPECT_TRUE(payment.isSuccessful());
    EXPECT_TRUE(payment.paidAt().has_value());
    payment.refund();
    EXPECT_FALSE(payment.isSuccessful());
    EXPECT_THROW(payment.refund(), PaymentException);
    EXPECT_THROW(payment.process(date()), PaymentException);
}

TEST(PaymentTest, RejectsInvalidData) {
    EXPECT_THROW(Payment("", Money(1.0, "RUB"), "Card"), TransportException);
    EXPECT_THROW(Payment("INV", Money(1.0, "RUB"), ""), TransportException);
    EXPECT_THROW(Payment("INV", Money(0.0, "RUB"), "Card"), PaymentException);
    EXPECT_THROW(Payment("INV", Money(-1.0, "RUB"), "Card"), PaymentException);

    Payment pending("INV", Money(1.0, "RUB"), "Card");
    EXPECT_THROW(pending.refund(), PaymentException);
}

TEST(ExpenseTrackerTest, AggregatesExpenses) {
    ExpenseTracker tracker;
    tracker.addExpense("Fuel", Money(100.0, "RUB"));
    tracker.addExpense("Fuel", Money(50.0, "RUB"));
    tracker.addExpense("Repair", Money(300.0, "RUB"));

    EXPECT_EQ(tracker.size(), 3);
    EXPECT_NEAR(tracker.total().amount(), 450.0, 1e-9);
    EXPECT_NEAR(tracker.byCategory("Fuel").amount(), 150.0, 1e-9);
    EXPECT_NEAR(tracker.byCategory("Unknown").amount(), 0.0, 1e-9);
    EXPECT_NEAR(tracker.largest().amount(), 300.0, 1e-9);
}

TEST(ExpenseTrackerTest, RejectsInvalidExpenses) {
    ExpenseTracker tracker;
    EXPECT_THROW(tracker.addExpense("", Money(1.0, "RUB")), TransportException);
    EXPECT_THROW(tracker.addExpense("Fuel", Money(-1.0, "RUB")), InvalidMoneyException);
    EXPECT_THROW(tracker.largest(), TransportException);
}

TEST(ProfitAnalyzerTest, ComputesProfitAndMargin) {
    ProfitAnalyzer analyzer;
    analyzer.recordIncome(Money(1000.0, "RUB"));
    analyzer.addExpense("Fuel", Money(200.0, "RUB"));

    EXPECT_NEAR(analyzer.grossProfit().amount(), 800.0, 1e-9);
    EXPECT_NEAR(analyzer.margin(), 80.0, 1e-9);
    EXPECT_NEAR(analyzer.monthlyProfit(2).amount(), 400.0, 1e-9);
    EXPECT_TRUE(analyzer.isProfitable());
    EXPECT_THROW(analyzer.monthlyProfit(0), TransportException);
    EXPECT_THROW(analyzer.recordIncome(Money(-1.0, "RUB")), InvalidMoneyException);
    EXPECT_THROW(analyzer.addExpense("", Money(1.0, "RUB")), TransportException);
}

TEST(ProfitAnalyzerTest, HandlesZeroAndNegativeProfit) {
    ProfitAnalyzer empty;
    EXPECT_NEAR(empty.margin(), 0.0, 1e-9);
    EXPECT_FALSE(empty.isProfitable());

    ProfitAnalyzer loss;
    loss.recordIncome(Money(100.0, "RUB"));
    loss.addExpense("Fuel", Money(200.0, "RUB"));
    EXPECT_TRUE(loss.grossProfit().isNegative());
    EXPECT_FALSE(loss.isProfitable());
}

TEST(TaxCalculatorTest, ComputesTaxes) {
    const TaxCalculator calculator(0.20, 0.13);
    EXPECT_NEAR(calculator.vat(Money(1000.0, "RUB")).amount(), 200.0, 1e-9);
    EXPECT_NEAR(calculator.incomeTax(Money(1000.0, "RUB")).amount(), 130.0, 1e-9);
    EXPECT_NEAR(calculator.netIncome(Money(1000.0, "RUB")).amount(), 870.0, 1e-9);
    EXPECT_DOUBLE_EQ(calculator.vatRate(), 0.20);
    EXPECT_DOUBLE_EQ(calculator.incomeTaxRate(), 0.13);
}

TEST(TaxCalculatorTest, RejectsInvalidRates) {
    EXPECT_THROW(TaxCalculator(-0.1, 0.13), TransportException);
    EXPECT_THROW(TaxCalculator(0.20, 1.1), TransportException);
    EXPECT_THROW(TaxCalculator(0.20, std::numeric_limits<double>::quiet_NaN()),
                 TransportException);
}

TEST(CompanyFinanceTest, RecordsIncomeAndExpenses) {
    CompanyFinance finance(bankAccount(10000.0), taxCalculator());
    Invoice invoice("INV-1", 1, 1, Money(1000.0, "RUB"),
                    date(), date(10, 1, 2026));
    finance.addInvoice(invoice);
    EXPECT_EQ(finance.unpaidInvoices().size(), 1);

    Payment payment("INV-1", Money(1000.0, "RUB"), "Card");
    payment.process(date());
    finance.recordIncome(payment);
    EXPECT_EQ(finance.payments().size(), 1);
    EXPECT_NEAR(finance.balance().amount(), 11000.0, 1e-9);

    Payment pending("INV-2", Money(500.0, "RUB"), "Card");
    finance.recordIncome(pending);
    EXPECT_EQ(finance.payments().size(), 2);
    EXPECT_NEAR(finance.balance().amount(), 11000.0, 1e-9);

    finance.recordExpense("Fuel", Money(1000.0, "RUB"));
    EXPECT_NEAR(finance.balance().amount(), 10000.0, 1e-9);
    EXPECT_THROW(finance.recordExpense("Rent", Money(1000000.0, "RUB")),
                 InsufficientFundsException);

    EXPECT_NE(finance.monthlyReport().find("balance"), std::string::npos);
    EXPECT_NEAR(finance.taxReport(Money(1000.0, "RUB")).amount(), 870.0, 1e-9);
}

} // namespace
