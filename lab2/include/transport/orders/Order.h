#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "transport/Enums.h"
#include "transport/core/Money.h"
#include "transport/core/TransportDate.h"
#include "transport/orders/Route.h"

namespace transport {

class Cargo;
class Client;
class Driver;
class Vehicle;

class Order {
public:
    Order(int64_t orderId, std::shared_ptr<Client> client, std::shared_ptr<Cargo> cargo,
          Route route, TransportDate createdOn, TransportDate deadline);
    virtual ~Order() = default;

    [[nodiscard]] int64_t orderId() const noexcept { return orderId_; }
    [[nodiscard]] const std::shared_ptr<Client>& client() const noexcept { return client_; }
    [[nodiscard]] const std::shared_ptr<Cargo>& cargo() const noexcept { return cargo_; }
    [[nodiscard]] const Route& route() const noexcept { return route_; }
    [[nodiscard]] const Money& price() const noexcept { return price_; }
    [[nodiscard]] OrderStatus status() const noexcept { return status_; }
    [[nodiscard]] const TransportDate& createdOn() const noexcept { return createdOn_; }
    [[nodiscard]] const TransportDate& deadline() const noexcept { return deadline_; }
    [[nodiscard]] const std::optional<int64_t>& driverId() const noexcept { return driverId_; }
    [[nodiscard]] const std::optional<std::string>& vehiclePlate() const noexcept { return vehiclePlate_; }

    [[nodiscard]] virtual Money calculatePrice();
    void assignDriver(const Driver& driver);
    void assignVehicle(const Vehicle& vehicle);
    void changeStatus(OrderStatus status);
    void cancel();
    [[nodiscard]] bool isDeliverable() const noexcept;
    [[nodiscard]] bool isOverdue(const TransportDate& today) const noexcept;

protected:
    void setPrice(const Money& price) noexcept;

private:
    int64_t orderId_ = 0;
    std::shared_ptr<Client> client_;
    std::shared_ptr<Cargo> cargo_;
    Route route_;
    Money price_{0.0, "RUB"};
    OrderStatus status_ = OrderStatus::Created;
    TransportDate createdOn_;
    TransportDate deadline_;
    std::optional<int64_t> driverId_;
    std::optional<std::string> vehiclePlate_;
};

} // namespace transport
