#pragma once

#include <cstdint>
#include <string>

#include "transport/core/TransportDate.h"

namespace transport {

class CustomerFeedback {
public:
    CustomerFeedback() = default;
    CustomerFeedback(int64_t clientId, int64_t orderId, int rating,
                     std::string comment, TransportDate submittedOn);

    [[nodiscard]] int64_t clientId() const noexcept { return clientId_; }
    [[nodiscard]] int64_t orderId() const noexcept { return orderId_; }
    [[nodiscard]] int rating() const noexcept { return rating_; }
    [[nodiscard]] const std::string& comment() const noexcept { return comment_; }
    [[nodiscard]] const TransportDate& submittedOn() const noexcept { return submittedOn_; }

    [[nodiscard]] bool isPositive() const noexcept;
    [[nodiscard]] std::string summary() const;

private:
    int64_t clientId_ = 0;
    int64_t orderId_ = 0;
    int rating_ = 0;
    std::string comment_;
    TransportDate submittedOn_;
};

} // namespace transport
