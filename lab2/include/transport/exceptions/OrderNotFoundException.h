#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class OrderNotFoundException : public TransportException {
public:
    explicit OrderNotFoundException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
