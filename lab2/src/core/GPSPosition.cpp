#include "transport/core/GPSPosition.h"

#include "transport/Constants.h"
#include "transport/exceptions/InvalidLocationException.h"

#include <cmath>

namespace transport {
namespace {

constexpr double kMinLatitude = -90.0;
constexpr double kMaxLatitude = 90.0;
constexpr double kMinLongitude = -180.0;
constexpr double kMaxLongitude = 180.0;

double toRadians(double degrees) noexcept {
    return degrees * 3.14159265358979323846 / 180.0;
}

} // namespace

GPSPosition::GPSPosition(double latitude, double longitude)
    : latitude_(latitude), longitude_(longitude) {
    if (!isValid()) {
        throw InvalidLocationException("GPS coordinates are out of range");
    }
}

bool GPSPosition::isValid() const noexcept {
    return std::isfinite(latitude_) && std::isfinite(longitude_) &&
           latitude_ >= kMinLatitude && latitude_ <= kMaxLatitude &&
           longitude_ >= kMinLongitude && longitude_ <= kMaxLongitude;
}

double GPSPosition::distanceTo(const GPSPosition& other) const {
    const double lat1 = toRadians(latitude_);
    const double lat2 = toRadians(other.latitude_);
    const double deltaLat = toRadians(other.latitude_ - latitude_);
    const double deltaLon = toRadians(other.longitude_ - longitude_);

    const double a = std::sin(deltaLat / 2.0) * std::sin(deltaLat / 2.0) +
                     std::cos(lat1) * std::cos(lat2) *
                     std::sin(deltaLon / 2.0) * std::sin(deltaLon / 2.0);
    const double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return constants::kEarthRadiusKm * c;
}

bool GPSPosition::approxEquals(const GPSPosition& other, double epsilon) const noexcept {
    return std::abs(latitude_ - other.latitude_) <= epsilon &&
           std::abs(longitude_ - other.longitude_) <= epsilon;
}

} // namespace transport
