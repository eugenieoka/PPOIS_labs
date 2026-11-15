#pragma once

#include <memory>
#include <string>

#include "transport/fleet/Vehicle.h"

namespace transport {

class Trailer;

class Truck : public Vehicle {
public:
    Truck(std::string plateNumber, double capacityKg, double capacityM3,
          double mileageKm, double baseRatePerKm, Machine machine, int axles);

    [[nodiscard]] int axles() const noexcept { return axles_; }
    [[nodiscard]] const std::shared_ptr<Trailer>& trailer() const noexcept { return trailer_; }
    [[nodiscard]] double maxPayload() const noexcept;

    void attachTrailer(std::shared_ptr<Trailer> trailer);
    void detachTrailer();
    [[nodiscard]] std::string typeName() const override;

private:
    int axles_ = 2;
    std::shared_ptr<Trailer> trailer_;
};

} // namespace transport
