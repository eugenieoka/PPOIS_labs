#include "transport/analytics/TrafficInfo.h"

#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {
namespace {

constexpr int kMinCongestionLevel = 1;
constexpr int kMaxCongestionLevel = 5;
constexpr int kSevereCongestionLevel = 4;
constexpr double kMinutesPerHour = 60.0;

} // namespace

TrafficInfo::TrafficInfo(std::string region, int delayMinutes, int congestionLevel)
    : region_(std::move(region)),
      delayMinutes_(delayMinutes),
      congestionLevel_(congestionLevel) {
    if (region_.empty()) {
        throw TransportException("Traffic region must not be empty");
    }
    if (delayMinutes_ < 0) {
        throw TransportException("Traffic delay must not be negative");
    }
    if (congestionLevel_ < kMinCongestionLevel || congestionLevel_ > kMaxCongestionLevel) {
        throw TransportException("Congestion level must be between 1 and 5");
    }
}

double TrafficInfo::delayFactor() const noexcept {
    return 1.0 + static_cast<double>(delayMinutes_) / kMinutesPerHour;
}

double TrafficInfo::adjustedDuration(double baseMinutes) const {
    if (baseMinutes < 0.0) {
        throw TransportException("Base duration must not be negative");
    }
    return baseMinutes * delayFactor();
}

bool TrafficInfo::isSevere() const noexcept {
    return congestionLevel_ >= kSevereCongestionLevel;
}

} // namespace transport
