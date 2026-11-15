#pragma once

#include "transport/exceptions/TransportException.h"

#include <string>

namespace transport {

class InvalidMoneyException : public TransportException {
public:
    explicit InvalidMoneyException(const std::string& message)
        : TransportException(message) {}
};

} // namespace transport
