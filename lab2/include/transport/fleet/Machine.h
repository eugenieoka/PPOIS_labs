#pragma once

#include <string>

namespace transport {

class Machine {
public:
    Machine() = default;
    Machine(std::string serialNumber, std::string fuelType, double fuelPerHour);

    [[nodiscard]] const std::string& serialNumber() const noexcept { return serialNumber_; }
    [[nodiscard]] const std::string& fuelType() const noexcept { return fuelType_; }
    [[nodiscard]] bool isRunning() const noexcept { return running_; }
    [[nodiscard]] double fuelPerHour() const noexcept { return fuelPerHour_; }

    void start();
    void stop();
    [[nodiscard]] double consumeFuel(double hours) const;

private:
    std::string serialNumber_;
    std::string fuelType_;
    bool running_ = false;
    double fuelPerHour_ = 0.0;
};

} // namespace transport
