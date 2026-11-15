#pragma once

#include <string>
#include <vector>

#include "transport/analytics/FuelConsumption.h"
#include "transport/analytics/PerformanceMetric.h"
#include "transport/analytics/TrafficInfo.h"

namespace transport {

class Metrics {
public:
    Metrics(FuelConsumption fuel, TrafficInfo traffic);

    void addMetric(const PerformanceMetric& metric);
    [[nodiscard]] double averageValue() const noexcept;
    [[nodiscard]] std::string summarize() const;

    [[nodiscard]] const std::vector<PerformanceMetric>& performanceMetrics() const noexcept {
        return performanceMetrics_;
    }
    [[nodiscard]] const FuelConsumption& fuelConsumption() const noexcept { return fuel_; }
    [[nodiscard]] const TrafficInfo& trafficInfo() const noexcept { return traffic_; }

private:
    std::vector<PerformanceMetric> performanceMetrics_;
    FuelConsumption fuel_;
    TrafficInfo traffic_;
};

} // namespace transport
