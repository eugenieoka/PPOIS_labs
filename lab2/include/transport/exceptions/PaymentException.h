#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class PaymentException : public TransportException {
public:
    explicit PaymentException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
