#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class InsufficientFundsException : public TransportException {
public:
    explicit InsufficientFundsException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
