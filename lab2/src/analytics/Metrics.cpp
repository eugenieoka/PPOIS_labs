#include "transport/analytics/Metrics.h"

#include <sstream>
#include <utility>

namespace transport {

Metrics::Metrics(FuelConsumption fuel, TrafficInfo traffic)
    : fuel_(std::move(fuel)), traffic_(std::move(traffic)) {}

void Metrics::addMetric(const PerformanceMetric& metric) {
    performanceMetrics_.push_back(metric);
}

double Metrics::averageValue() const noexcept {
    if (performanceMetrics_.empty()) {
        return 0.0;
    }
    double total = 0.0;
    for (const PerformanceMetric& metric : performanceMetrics_) {
        total += metric.value();
    }
    return total / static_cast<double>(performanceMetrics_.size());
}

std::string Metrics::summarize() const {
    std::ostringstream out;
    out << "metrics=" << performanceMetrics_.size()
        << ", average=" << averageValue()
        << ", fuel=" << fuel_.per100Km() << " l/100km"
        << ", traffic delay=" << traffic_.delayFactor();
    return out.str();
}

} // namespace transport
