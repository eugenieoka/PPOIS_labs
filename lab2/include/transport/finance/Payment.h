#pragma once

#include <optional>
#include <string>

#include "transport/Enums.h"
#include "transport/core/Money.h"
#include "transport/core/TransportDate.h"

namespace transport {

class Payment {
public:
    Payment() = default;
    Payment(std::string invoiceNumber, Money amount, std::string method);

    [[nodiscard]] const std::string& invoiceNumber() const noexcept { return invoiceNumber_; }
    [[nodiscard]] const Money& amount() const noexcept { return amount_; }
    [[nodiscard]] const std::string& method() const noexcept { return method_; }
    [[nodiscard]] PaymentStatus status() const noexcept { return status_; }
    [[nodiscard]] const std::optional<TransportDate>& paidAt() const noexcept { return paidAt_; }

    void process(const TransportDate& date);
    void refund();
    [[nodiscard]] bool isSuccessful() const noexcept;

private:
    std::string invoiceNumber_;
    Money amount_{0.0, "RUB"};
    std::string method_;
    PaymentStatus status_ = PaymentStatus::Pending;
    std::optional<TransportDate> paidAt_;
};

} // namespace transport
