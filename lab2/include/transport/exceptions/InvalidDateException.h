#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class InvalidDateException : public TransportException {
public:
    explicit InvalidDateException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
