#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class VehicleNotAvailableException : public TransportException {
public:
    explicit VehicleNotAvailableException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
