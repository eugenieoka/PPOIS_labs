#include "transport/orders/Delivery.h"

#include "transport/Constants.h"
#include "transport/exceptions/InvalidOrderStateException.h"
#include "transport/exceptions/TransportException.h"
#include "transport/fleet/Vehicle.h"
#include "transport/orders/Order.h"
#include "transport/staff/Driver.h"

#include <utility>

namespace transport {

Delivery::Delivery(int64_t deliveryId, std::shared_ptr<Order> order,
                   std::shared_ptr<Driver> driver, std::shared_ptr<Vehicle> vehicle)
    : deliveryId_(deliveryId),
      order_(std::move(order)),
      driver_(std::move(driver)),
      vehicle_(std::move(vehicle)) {
    if (deliveryId_ <= 0) {
        throw TransportException("Delivery id must be positive");
    }
    if (!order_ || !driver_ || !vehicle_) {
        throw TransportException("Delivery requires order, driver and vehicle");
    }
}

void Delivery::start(const TransportDate& date) {
    if (status_ != DeliveryStatus::Planned) {
        throw InvalidOrderStateException("Delivery has already been started");
    }
    if (!order_->isDeliverable()) {
        throw InvalidOrderStateException("Order is not ready for delivery");
    }
    driver_->assign();
    vehicle_->changeStatus(VehicleStatus::OnRoute);
    order_->changeStatus(OrderStatus::InTransit);
    status_ = DeliveryStatus::InProgress;
    startedAt_ = date;
}

void Delivery::complete(const TransportDate& date) {
    if (status_ != DeliveryStatus::InProgress) {
        throw InvalidOrderStateException("Delivery is not in progress");
    }
    if (!startedAt_ || date.isBefore(*startedAt_)) {
        throw InvalidOrderStateException("Completion date precedes delivery start");
    }
    status_ = DeliveryStatus::Completed;
    completedAt_ = date;
    order_->changeStatus(OrderStatus::Delivered);
    driver_->completeDelivery();
    vehicle_->changeStatus(VehicleStatus::Available);
}

void Delivery::fail() {
    if (status_ != DeliveryStatus::InProgress) {
        throw InvalidOrderStateException("Only an in-progress delivery can fail");
    }
    status_ = DeliveryStatus::Failed;
    driver_->completeDelivery();
    vehicle_->changeStatus(VehicleStatus::Available);
    order_->cancel();
}

double Delivery::durationHours() const {
    if (!startedAt_ || !completedAt_) {
        throw InvalidOrderStateException("Delivery has no complete duration");
    }
    return static_cast<double>(startedAt_->daysUntil(*completedAt_)) *
           constants::kHoursPerDay;
}

bool Delivery::isOnTime() const {
    if (!completedAt_) {
        return false;
    }
    return !order_->deadline().isBefore(*completedAt_);
}

} // namespace transport
