#pragma once

#include <string>
#include <vector>

#include "transport/orders/RoutePoint.h"

namespace transport {

class Route {
public:
    Route() = default;
    explicit Route(std::string name);

    [[nodiscard]] const std::string& name() const noexcept { return name_; }
    [[nodiscard]] const std::vector<RoutePoint>& points() const noexcept { return points_; }
    [[nodiscard]] double totalDistance() const noexcept { return distanceKm_; }

    void addPoint(const RoutePoint& point);
    [[nodiscard]] double estimatedDuration(double averageSpeedKmh) const;
    [[nodiscard]] bool contains(const Address& address) const;
    void optimize();

private:
    std::string name_;
    std::vector<RoutePoint> points_;
    double distanceKm_ = 0.0;

    void recalculateDistance() noexcept;
};

} // namespace transport
