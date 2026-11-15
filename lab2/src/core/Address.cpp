#include "transport/core/Address.h"

#include "transport/exceptions/InvalidLocationException.h"

#include <sstream>
#include <utility>

namespace transport {

Address::Address(std::string street, std::string city, std::string postalCode, std::string building)
    : street_(std::move(street)),
      city_(std::move(city)),
      postalCode_(std::move(postalCode)),
      building_(std::move(building)) {
    if (street_.empty() || city_.empty() || postalCode_.empty() || building_.empty()) {
        throw InvalidLocationException("Address fields must not be empty");
    }
}

std::string Address::format() const {
    std::ostringstream out;
    out << city_ << ", " << street_ << ", " << building_ << ", " << postalCode_;
    return out.str();
}

bool Address::sameCity(const Address& other) const noexcept {
    return city_ == other.city_;
}

} // namespace transport
