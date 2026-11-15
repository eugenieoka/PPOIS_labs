#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class DriverNotAvailableException : public TransportException {
public:
    explicit DriverNotAvailableException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
