#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class VehicleOverloadException : public TransportException {
public:
    explicit VehicleOverloadException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
