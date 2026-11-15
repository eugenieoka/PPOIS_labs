#pragma once

namespace transport {

class GPSPosition {
public:
    GPSPosition() = default;
    GPSPosition(double latitude, double longitude);

    [[nodiscard]] double latitude() const noexcept { return latitude_; }
    [[nodiscard]] double longitude() const noexcept { return longitude_; }
    [[nodiscard]] bool isValid() const noexcept;
    [[nodiscard]] double distanceTo(const GPSPosition& other) const;
    [[nodiscard]] bool approxEquals(const GPSPosition& other, double epsilon = 1e-6) const noexcept;

private:
    double latitude_ = 0.0;
    double longitude_ = 0.0;
};

} // namespace transport
