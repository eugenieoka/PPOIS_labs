#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "transport/Enums.h"
#include "transport/core/TransportDate.h"

namespace transport {

class Driver;
class Order;
class Vehicle;

class Delivery {
public:
    Delivery(int64_t deliveryId, std::shared_ptr<Order> order,
             std::shared_ptr<Driver> driver, std::shared_ptr<Vehicle> vehicle);

    [[nodiscard]] int64_t deliveryId() const noexcept { return deliveryId_; }
    [[nodiscard]] const std::shared_ptr<Order>& order() const noexcept { return order_; }
    [[nodiscard]] const std::shared_ptr<Driver>& driver() const noexcept { return driver_; }
    [[nodiscard]] const std::shared_ptr<Vehicle>& vehicle() const noexcept { return vehicle_; }
    [[nodiscard]] DeliveryStatus status() const noexcept { return status_; }
    [[nodiscard]] const std::optional<TransportDate>& startedAt() const noexcept { return startedAt_; }
    [[nodiscard]] const std::optional<TransportDate>& completedAt() const noexcept { return completedAt_; }

    void start(const TransportDate& date);
    void complete(const TransportDate& date);
    void fail();
    [[nodiscard]] double durationHours() const;
    [[nodiscard]] bool isOnTime() const;

private:
    int64_t deliveryId_ = 0;
    std::shared_ptr<Order> order_;
    std::shared_ptr<Driver> driver_;
    std::shared_ptr<Vehicle> vehicle_;
    DeliveryStatus status_ = DeliveryStatus::Planned;
    std::optional<TransportDate> startedAt_;
    std::optional<TransportDate> completedAt_;
};

} // namespace transport
