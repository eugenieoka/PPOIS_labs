#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class InvalidOrderStateException : public TransportException {
public:
    explicit InvalidOrderStateException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
