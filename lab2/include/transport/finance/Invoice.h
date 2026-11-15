#pragma once

#include <cstdint>
#include <string>

#include "transport/core/Money.h"
#include "transport/core/TransportDate.h"

namespace transport {

class Invoice {
public:
    Invoice() = default;
    Invoice(std::string number, int64_t orderId, int64_t clientId,
            Money amount, TransportDate issuedOn, TransportDate dueOn);

    [[nodiscard]] const std::string& number() const noexcept { return number_; }
    [[nodiscard]] int64_t orderId() const noexcept { return orderId_; }
    [[nodiscard]] int64_t clientId() const noexcept { return clientId_; }
    [[nodiscard]] const Money& amount() const noexcept { return amount_; }
    [[nodiscard]] const TransportDate& issuedOn() const noexcept { return issuedOn_; }
    [[nodiscard]] const TransportDate& dueOn() const noexcept { return dueOn_; }
    [[nodiscard]] bool isPaid() const noexcept { return paid_; }

    void markPaid() noexcept;
    [[nodiscard]] Money remaining() const;
    [[nodiscard]] bool isOverdue(const TransportDate& today) const noexcept;
    [[nodiscard]] Money totalWithVat(double vatRate) const;
    void pay(const Money& amount);

private:
    std::string number_;
    int64_t orderId_ = 0;
    int64_t clientId_ = 0;
    Money amount_{0.0, "RUB"};
    TransportDate issuedOn_;
    TransportDate dueOn_;
    bool paid_ = false;
};

} // namespace transport
