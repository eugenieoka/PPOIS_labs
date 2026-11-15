#include "transport/orders/RoutePoint.h"

#include "transport/exceptions/TransportException.h"

#include <sstream>
#include <utility>

namespace transport {

RoutePoint::RoutePoint(Address address, GPSPosition position, int sequence, int64_t orderId)
    : address_(std::move(address)), position_(position), sequence_(sequence), orderId_(orderId) {
    if (sequence_ < 0) {
        throw TransportException("Route point sequence must not be negative");
    }
    if (orderId_ <= 0) {
        throw TransportException("Route point order id must be positive");
    }
}

bool RoutePoint::sameAs(const RoutePoint& other) const {
    return address_.format() == other.address_.format() &&
           position_.approxEquals(other.position_) &&
           sequence_ == other.sequence_;
}

double RoutePoint::distanceTo(const RoutePoint& other) const {
    return position_.distanceTo(other.position_);
}

std::string RoutePoint::describe() const {
    std::ostringstream out;
    out << sequence_ << ". " << address_.format();
    return out.str();
}

} // namespace transport
