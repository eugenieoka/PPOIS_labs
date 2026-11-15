#include "transport/orders/Order.h"

#include "transport/Constants.h"
#include "transport/cargo/Cargo.h"
#include "transport/exceptions/DriverNotAvailableException.h"
#include "transport/exceptions/InvalidOrderStateException.h"
#include "transport/exceptions/TransportException.h"
#include "transport/exceptions/VehicleNotAvailableException.h"
#include "transport/exceptions/VehicleOverloadException.h"
#include "transport/fleet/Vehicle.h"
#include "transport/orders/Client.h"
#include "transport/staff/Driver.h"

#include <utility>

namespace transport {
namespace {

bool canTransition(OrderStatus from, OrderStatus to) noexcept {
    if (from == to) {
        return true;
    }
    switch (from) {
        case OrderStatus::Created:
            return to == OrderStatus::Assigned || to == OrderStatus::Cancelled;
        case OrderStatus::Assigned:
            return to == OrderStatus::InTransit || to == OrderStatus::Cancelled;
        case OrderStatus::InTransit:
            return to == OrderStatus::Delivered || to == OrderStatus::Cancelled;
        case OrderStatus::Delivered:
        case OrderStatus::Cancelled:
            return false;
    }
    return false;
}

} // namespace

Order::Order(int64_t orderId, std::shared_ptr<Client> client, std::shared_ptr<Cargo> cargo,
             Route route, TransportDate createdOn, TransportDate deadline)
    : orderId_(orderId),
      client_(std::move(client)),
      cargo_(std::move(cargo)),
      route_(std::move(route)),
      createdOn_(createdOn),
      deadline_(deadline) {
    if (orderId_ <= 0) {
        throw TransportException("Order id must be positive");
    }
    if (!client_ || !cargo_) {
        throw TransportException("Order requires a client and cargo");
    }
    if (deadline_.isBefore(createdOn_)) {
        throw TransportException("Order deadline must not precede creation date");
    }
}

Money Order::calculatePrice() {
    if (status_ == OrderStatus::Cancelled) {
        throw InvalidOrderStateException("Cannot calculate price for a cancelled order");
    }
    const Money distanceCost(route_.totalDistance() * constants::kPricePerKm, "RUB");
    const Money base = cargo_->handlingCost().add(distanceCost);
    price_ = base.multiply(1.0 - client_->discountRate());
    return price_;
}

void Order::assignDriver(const Driver& driver) {
    if (status_ != OrderStatus::Created && status_ != OrderStatus::Assigned) {
        throw InvalidOrderStateException("Driver can only be assigned to a new order");
    }
    if (!driver.isAvailable()) {
        throw DriverNotAvailableException("Driver is not available");
    }
    driverId_ = driver.employeeId();
    changeStatus(OrderStatus::Assigned);
}

void Order::assignVehicle(const Vehicle& vehicle) {
    if (status_ != OrderStatus::Created && status_ != OrderStatus::Assigned) {
        throw InvalidOrderStateException("Vehicle can only be assigned to a new order");
    }
    if (!vehicle.isAvailable()) {
        throw VehicleNotAvailableException("Vehicle is not available");
    }
    if (!vehicle.canCarry(*cargo_)) {
        throw VehicleOverloadException("Vehicle cannot carry the cargo");
    }
    vehiclePlate_ = vehicle.plateNumber();
    changeStatus(OrderStatus::Assigned);
}

void Order::changeStatus(OrderStatus status) {
    if (!canTransition(status_, status)) {
        throw InvalidOrderStateException("Invalid order status transition");
    }
    status_ = status;
}

void Order::cancel() {
    if (status_ == OrderStatus::Cancelled) {
        throw InvalidOrderStateException("Order is already cancelled");
    }
    changeStatus(OrderStatus::Cancelled);
}

bool Order::isDeliverable() const noexcept {
    return status_ == OrderStatus::Assigned && driverId_.has_value() && vehiclePlate_.has_value();
}

bool Order::isOverdue(const TransportDate& today) const noexcept {
    return status_ != OrderStatus::Delivered && status_ != OrderStatus::Cancelled &&
           today.isAfter(deadline_);
}

void Order::setPrice(const Money& price) noexcept {
    price_ = price;
}

} // namespace transport
