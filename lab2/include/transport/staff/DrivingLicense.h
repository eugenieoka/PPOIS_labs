#pragma once

#include <string>

#include "transport/core/TransportDate.h"

namespace transport {

class DrivingLicense {
public:
    DrivingLicense() = default;
    DrivingLicense(std::string number, std::string category,
                   TransportDate issuedOn, TransportDate expiresOn);

    [[nodiscard]] const std::string& number() const noexcept { return number_; }
    [[nodiscard]] const std::string& category() const noexcept { return category_; }
    [[nodiscard]] const TransportDate& issuedOn() const noexcept { return issuedOn_; }
    [[nodiscard]] const TransportDate& expiresOn() const noexcept { return expiresOn_; }

    [[nodiscard]] bool isValidOn(const TransportDate& date) const noexcept;
    [[nodiscard]] bool coversCategory(const std::string& category) const noexcept;
    void renew(const TransportDate& newExpiry);

private:
    std::string number_;
    std::string category_;
    TransportDate issuedOn_;
    TransportDate expiresOn_;
};

} // namespace transport
