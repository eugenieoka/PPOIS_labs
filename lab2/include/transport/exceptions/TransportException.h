#pragma once

#include <stdexcept>
#include <string>

namespace transport {

class TransportException : public std::runtime_error {
public:
    explicit TransportException(const std::string& message)
        : std::runtime_error(message) {}
};

} // namespace transport
