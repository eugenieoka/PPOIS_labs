#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "transport/orders/Delivery.h"

namespace transport {

class Dispatcher;
class Driver;
class Order;
class Vehicle;

class LogisticsManager {
public:
    void addDispatcher(std::shared_ptr<Dispatcher> dispatcher);
    [[nodiscard]] Delivery& assignDelivery(int64_t deliveryId,
                                           std::shared_ptr<Order> order,
                                           std::shared_ptr<Driver> driver,
                                           std::shared_ptr<Vehicle> vehicle);
    void completeDelivery(int64_t deliveryId, const TransportDate& date);
    [[nodiscard]] Delivery& findDelivery(int64_t deliveryId);
    [[nodiscard]] const std::vector<Delivery>& deliveries() const noexcept { return deliveries_; }
    [[nodiscard]] std::string dailyReport() const;

private:
    std::vector<Delivery> deliveries_;
    std::vector<std::shared_ptr<Dispatcher>> dispatchers_;
};

} // namespace transport
