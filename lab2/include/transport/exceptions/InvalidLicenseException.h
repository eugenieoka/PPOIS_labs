#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class InvalidLicenseException : public TransportException {
public:
    explicit InvalidLicenseException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
