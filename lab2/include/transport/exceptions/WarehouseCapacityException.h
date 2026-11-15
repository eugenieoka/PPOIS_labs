#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class WarehouseCapacityException : public TransportException {
public:
    explicit WarehouseCapacityException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
