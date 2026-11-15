#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class InvalidCargoDataException : public TransportException {
public:
    explicit InvalidCargoDataException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
