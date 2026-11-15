#include "transport/staff/DrivingLicense.h"

#include "transport/exceptions/InvalidLicenseException.h"

#include <utility>

namespace transport {
namespace {

bool isKnownCategory(const std::string& category) {
    return category == "A" || category == "B" || category == "C" ||
           category == "D" || category == "E";
}

} // namespace

DrivingLicense::DrivingLicense(std::string number, std::string category,
                               TransportDate issuedOn, TransportDate expiresOn)
    : number_(std::move(number)),
      category_(std::move(category)),
      issuedOn_(issuedOn),
      expiresOn_(expiresOn) {
    if (number_.empty()) {
        throw InvalidLicenseException("License number must not be empty");
    }
    if (!isKnownCategory(category_)) {
        throw InvalidLicenseException("Unknown driving license category");
    }
    if (expiresOn_.isBefore(issuedOn_)) {
        throw InvalidLicenseException("License expiry must not precede issue date");
    }
}

bool DrivingLicense::isValidOn(const TransportDate& date) const noexcept {
    return !date.isBefore(issuedOn_) && !date.isAfter(expiresOn_);
}

bool DrivingLicense::coversCategory(const std::string& category) const noexcept {
    if (category_ == category) {
        return true;
    }
    return category_ == "E" && (category == "A" || category == "B" ||
                               category == "C" || category == "D");
}

void DrivingLicense::renew(const TransportDate& newExpiry) {
    if (!newExpiry.isAfter(expiresOn_)) {
        throw InvalidLicenseException("New expiry must be after the current expiry");
    }
    expiresOn_ = newExpiry;
}

} // namespace transport
