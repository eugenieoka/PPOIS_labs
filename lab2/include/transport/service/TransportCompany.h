#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "transport/core/Address.h"
#include "transport/finance/CompanyFinance.h"
#include "transport/orders/TrackingSystem.h"
#include "transport/service/CustomerService.h"
#include "transport/warehouse/Warehouse.h"

namespace transport {

class Client;
class Driver;
class Employee;
class Order;
class Vehicle;

class TransportCompany {
public:
    TransportCompany(std::string name, Address address,
                     BankAccount account, TaxCalculator taxCalculator);

    [[nodiscard]] const std::string& name() const noexcept { return name_; }
    [[nodiscard]] const Address& address() const noexcept { return address_; }
    [[nodiscard]] const std::vector<std::shared_ptr<Employee>>& employees() const noexcept {
        return employees_;
    }
    [[nodiscard]] const std::vector<std::shared_ptr<Vehicle>>& fleet() const noexcept {
        return fleet_;
    }
    [[nodiscard]] const std::vector<std::shared_ptr<Client>>& clients() const noexcept {
        return clients_;
    }
    [[nodiscard]] const std::vector<std::shared_ptr<Order>>& orders() const noexcept {
        return orders_;
    }
    [[nodiscard]] const std::vector<Warehouse>& warehouses() const noexcept { return warehouses_; }
    [[nodiscard]] const CompanyFinance& finance() const noexcept { return finance_; }
    [[nodiscard]] const TrackingSystem& tracking() const noexcept { return tracking_; }
    [[nodiscard]] const CustomerService& customerService() const noexcept {
        return customerService_;
    }

    void hire(std::shared_ptr<Employee> employee);
    void addClient(std::shared_ptr<Client> client);
    void addVehicle(std::shared_ptr<Vehicle> vehicle);
    void addWarehouse(const Warehouse& warehouse);
    void placeOrder(std::shared_ptr<Order> order);

    void dispatchOrder(int64_t orderId, std::shared_ptr<Driver> driver,
                       std::shared_ptr<Vehicle> vehicle, const TransportDate& date);
    void cancelOrder(int64_t orderId, const TransportDate& date);

    [[nodiscard]] std::shared_ptr<Order> findOrder(int64_t orderId) const;
    [[nodiscard]] std::shared_ptr<Client> findClient(int64_t clientId) const;
    [[nodiscard]] std::string financialReport() const;
    [[nodiscard]] std::string qualityReport() const;

private:
    std::string name_;
    Address address_;
    std::vector<std::shared_ptr<Employee>> employees_;
    std::vector<std::shared_ptr<Vehicle>> fleet_;
    std::vector<std::shared_ptr<Client>> clients_;
    std::vector<std::shared_ptr<Order>> orders_;
    std::vector<Warehouse> warehouses_;
    CompanyFinance finance_;
    TrackingSystem tracking_;
    CustomerService customerService_;
};

} // namespace transport
