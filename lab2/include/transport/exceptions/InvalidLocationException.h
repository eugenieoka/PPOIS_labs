#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class InvalidLocationException : public TransportException {
public:
    explicit InvalidLocationException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
