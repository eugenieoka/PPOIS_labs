#pragma once

#include <string>

namespace transport {

class Address {
public:
    Address() = default;
    Address(std::string street, std::string city, std::string postalCode, std::string building);

    [[nodiscard]] const std::string& street() const noexcept { return street_; }
    [[nodiscard]] const std::string& city() const noexcept { return city_; }
    [[nodiscard]] const std::string& postalCode() const noexcept { return postalCode_; }
    [[nodiscard]] const std::string& building() const noexcept { return building_; }

    [[nodiscard]] std::string format() const;
    [[nodiscard]] bool sameCity(const Address& other) const noexcept;

private:
    std::string street_;
    std::string city_;
    std::string postalCode_;
    std::string building_;
};

} // namespace transport
