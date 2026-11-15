#include "transport/analytics/PerformanceMetric.h"

#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

PerformanceMetric::PerformanceMetric(std::string name, double value, double target)
    : name_(std::move(name)), value_(value), target_(target) {
    if (name_.empty()) {
        throw TransportException("Performance metric name must not be empty");
    }
}

bool PerformanceMetric::isOnTarget() const noexcept {
    return value_ >= target_;
}

double PerformanceMetric::deviation() const noexcept {
    return value_ - target_;
}

void PerformanceMetric::update(double value) noexcept {
    value_ = value;
}

} // namespace transport
