#include "transport/finance/Invoice.h"

#include "transport/exceptions/InvalidMoneyException.h"
#include "transport/exceptions/PaymentException.h"
#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

Invoice::Invoice(std::string number, int64_t orderId, int64_t clientId,
                 Money amount, TransportDate issuedOn, TransportDate dueOn)
    : number_(std::move(number)),
      orderId_(orderId),
      clientId_(clientId),
      amount_(std::move(amount)),
      issuedOn_(issuedOn),
      dueOn_(dueOn) {
    if (number_.empty()) {
        throw TransportException("Invoice number must not be empty");
    }
    if (orderId_ <= 0 || clientId_ <= 0) {
        throw TransportException("Invoice order and client ids must be positive");
    }
    if (amount_.isNegative()) {
        throw InvalidMoneyException("Invoice amount must not be negative");
    }
    if (dueOn_.isBefore(issuedOn_)) {
        throw TransportException("Invoice due date must not precede issue date");
    }
}

void Invoice::markPaid() noexcept {
    paid_ = true;
}

Money Invoice::remaining() const {
    return paid_ ? Money(0.0, amount_.currency()) : amount_;
}

bool Invoice::isOverdue(const TransportDate& today) const noexcept {
    return !paid_ && today.isAfter(dueOn_);
}

Money Invoice::totalWithVat(double vatRate) const {
    if (vatRate < 0.0 || vatRate > 1.0) {
        throw PaymentException("VAT rate must be between 0 and 1");
    }
    return amount_.multiply(1.0 + vatRate);
}

void Invoice::pay(const Money& amount) {
    if (paid_) {
        throw PaymentException("Invoice is already paid");
    }
    if (amount.isNegative()) {
        throw PaymentException("Payment amount must not be negative");
    }
    if (amount.amount() < amount_.amount()) {
        throw PaymentException("Payment amount is less than the invoice amount");
    }
    paid_ = true;
}

} // namespace transport
