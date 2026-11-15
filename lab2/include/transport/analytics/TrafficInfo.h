#pragma once

#include <string>

namespace transport {

class TrafficInfo {
public:
    TrafficInfo() = default;
    TrafficInfo(std::string region, int delayMinutes, int congestionLevel);

    [[nodiscard]] const std::string& region() const noexcept { return region_; }
    [[nodiscard]] int delayMinutes() const noexcept { return delayMinutes_; }
    [[nodiscard]] int congestionLevel() const noexcept { return congestionLevel_; }

    [[nodiscard]] double delayFactor() const noexcept;
    [[nodiscard]] double adjustedDuration(double baseMinutes) const;
    [[nodiscard]] bool isSevere() const noexcept;

private:
    std::string region_;
    int delayMinutes_ = 0;
    int congestionLevel_ = 1;
};

} // namespace transport
