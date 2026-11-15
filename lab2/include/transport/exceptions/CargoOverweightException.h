#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class CargoOverweightException : public TransportException {
public:
    explicit CargoOverweightException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
