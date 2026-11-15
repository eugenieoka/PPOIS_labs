#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class InvalidClientDataException : public TransportException {
public:
    explicit InvalidClientDataException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
