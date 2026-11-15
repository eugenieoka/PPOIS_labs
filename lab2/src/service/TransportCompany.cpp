#include "transport/service/TransportCompany.h"

#include "transport/exceptions/OrderNotFoundException.h"
#include "transport/exceptions/TransportException.h"
#include "transport/orders/Client.h"
#include "transport/orders/Order.h"
#include "transport/staff/Driver.h"
#include "transport/staff/Employee.h"
#include "transport/fleet/Vehicle.h"

#include <sstream>
#include <utility>

namespace transport {

TransportCompany::TransportCompany(std::string name, Address address,
                                   BankAccount account, TaxCalculator taxCalculator)
    : name_(std::move(name)),
      address_(std::move(address)),
      finance_(std::move(account), std::move(taxCalculator)) {
    if (name_.empty()) {
        throw TransportException("Company name must not be empty");
    }
}

void TransportCompany::hire(std::shared_ptr<Employee> employee) {
    if (!employee) {
        throw TransportException("Employee must not be null");
    }
    employees_.push_back(std::move(employee));
}

void TransportCompany::addClient(std::shared_ptr<Client> client) {
    if (!client) {
        throw TransportException("Client must not be null");
    }
    clients_.push_back(std::move(client));
    customerService_.registerClient(clients_.back());
}

void TransportCompany::addVehicle(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) {
        throw TransportException("Vehicle must not be null");
    }
    fleet_.push_back(std::move(vehicle));
}

void TransportCompany::addWarehouse(const Warehouse& warehouse) {
    warehouses_.push_back(warehouse);
}

void TransportCompany::placeOrder(std::shared_ptr<Order> order) {
    if (!order) {
        throw TransportException("Order must not be null");
    }
    orders_.push_back(std::move(order));
}

void TransportCompany::dispatchOrder(int64_t orderId, std::shared_ptr<Driver> driver,
                                     std::shared_ptr<Vehicle> vehicle,
                                     const TransportDate& date) {
    if (!driver || !vehicle) {
        throw TransportException("Driver and vehicle must not be null");
    }
    const std::shared_ptr<Order> order = findOrder(orderId);
    order->assignDriver(*driver);
    order->assignVehicle(*vehicle);
    const Money calculatedPrice = order->calculatePrice();
    (void)calculatedPrice;
    tracking_.addEvent(orderId, "Assigned", date);
}

void TransportCompany::cancelOrder(int64_t orderId, const TransportDate& date) {
    const std::shared_ptr<Order> order = findOrder(orderId);
    order->cancel();
    tracking_.addEvent(orderId, "Cancelled", date);
}

std::shared_ptr<Order> TransportCompany::findOrder(int64_t orderId) const {
    for (const auto& order : orders_) {
        if (order->orderId() == orderId) {
            return order;
        }
    }
    throw OrderNotFoundException("Order not found");
}

std::shared_ptr<Client> TransportCompany::findClient(int64_t clientId) const {
    for (const auto& client : clients_) {
        if (client->clientId() == clientId) {
            return client;
        }
    }
    throw OrderNotFoundException("Client not found");
}

std::string TransportCompany::financialReport() const {
    return finance_.monthlyReport();
}

std::string TransportCompany::qualityReport() const {
    std::ostringstream out;
    out << "feedback=" << customerService_.monitor().feedbackCount()
        << ", average rating=" << customerService_.monitor().averageRating();
    return out.str();
}

} // namespace transport
