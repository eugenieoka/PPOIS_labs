#pragma once

#include <cstdint>
#include <string>

#include "transport/core/Address.h"
#include "transport/core/GPSPosition.h"

namespace transport {

class RoutePoint {
public:
    RoutePoint() = default;
    RoutePoint(Address address, GPSPosition position, int sequence, int64_t orderId);

    [[nodiscard]] const Address& address() const noexcept { return address_; }
    [[nodiscard]] const GPSPosition& position() const noexcept { return position_; }
    [[nodiscard]] int sequence() const noexcept { return sequence_; }
    [[nodiscard]] int64_t orderId() const noexcept { return orderId_; }

    [[nodiscard]] bool sameAs(const RoutePoint& other) const;
    [[nodiscard]] double distanceTo(const RoutePoint& other) const;
    [[nodiscard]] std::string describe() const;

private:
    Address address_;
    GPSPosition position_;
    int sequence_ = 0;
    int64_t orderId_ = 0;
};

} // namespace transport
