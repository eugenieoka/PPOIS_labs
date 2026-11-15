#include "transport/finance/Payment.h"

#include "transport/exceptions/PaymentException.h"
#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

Payment::Payment(std::string invoiceNumber, Money amount, std::string method)
    : invoiceNumber_(std::move(invoiceNumber)), amount_(std::move(amount)), method_(std::move(method)) {
    if (invoiceNumber_.empty() || method_.empty()) {
        throw TransportException("Payment requires invoice number and method");
    }
    if (amount_.isNegative() || amount_.isZero()) {
        throw PaymentException("Payment amount must be positive");
    }
}

void Payment::process(const TransportDate& date) {
    if (status_ != PaymentStatus::Pending) {
        throw PaymentException("Payment has already been processed");
    }
    status_ = PaymentStatus::Completed;
    paidAt_ = date;
}

void Payment::refund() {
    if (status_ != PaymentStatus::Completed) {
        throw PaymentException("Only a completed payment can be refunded");
    }
    status_ = PaymentStatus::Refunded;
}

bool Payment::isSuccessful() const noexcept {
    return status_ == PaymentStatus::Completed;
}

} // namespace transport
