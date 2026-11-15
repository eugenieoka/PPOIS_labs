#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class InvalidRatingException : public TransportException {
public:
    explicit InvalidRatingException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
