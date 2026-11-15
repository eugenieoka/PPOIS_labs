#include "transport/orders/Route.h"

#include "transport/exceptions/TransportException.h"

#include <algorithm>
#include <utility>

namespace transport {

Route::Route(std::string name) : name_(std::move(name)) {
    if (name_.empty()) {
        throw TransportException("Route name must not be empty");
    }
}

void Route::addPoint(const RoutePoint& point) {
    points_.push_back(point);
    recalculateDistance();
}

double Route::estimatedDuration(double averageSpeedKmh) const {
    if (averageSpeedKmh <= 0.0) {
        throw TransportException("Average speed must be positive");
    }
    return distanceKm_ / averageSpeedKmh;
}

bool Route::contains(const Address& address) const {
    return std::any_of(points_.begin(), points_.end(),
                       [&address](const RoutePoint& point) {
                           return point.address().format() == address.format();
                       });
}

void Route::optimize() {
    std::sort(points_.begin(), points_.end(),
              [](const RoutePoint& left, const RoutePoint& right) {
                  return left.sequence() < right.sequence();
              });
    recalculateDistance();
}

void Route::recalculateDistance() noexcept {
    distanceKm_ = 0.0;
    for (std::size_t i = 1; i < points_.size(); ++i) {
        distanceKm_ += points_[i - 1].distanceTo(points_[i]);
    }
}

} // namespace transport
