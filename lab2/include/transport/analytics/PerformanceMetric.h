#pragma once

#include <string>

namespace transport {

class PerformanceMetric {
public:
    PerformanceMetric() = default;
    PerformanceMetric(std::string name, double value, double target);

    [[nodiscard]] const std::string& name() const noexcept { return name_; }
    [[nodiscard]] double value() const noexcept { return value_; }
    [[nodiscard]] double target() const noexcept { return target_; }

    [[nodiscard]] bool isOnTarget() const noexcept;
    [[nodiscard]] double deviation() const noexcept;
    void update(double value) noexcept;

private:
    std::string name_;
    double value_ = 0.0;
    double target_ = 0.0;
};

} // namespace transport
