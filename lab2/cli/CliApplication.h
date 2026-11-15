#pragma once

#include <memory>

#include "transport/finance/CompanyFinance.h"
#include "transport/service/ServiceManagement.h"
#include "transport/service/TransportCompany.h"
#include "transport/warehouse/StorageUnit.h"

namespace transport {
class Cargo;
class Client;
class Driver;
class Order;
class Vehicle;
class Warehouse;
} // namespace transport

namespace transport::cli {

class CliApplication {
public:
    CliApplication();
    int run();

private:
    void printMenu() const;
    [[nodiscard]] int readChoice() const;
    void printOverview() const;
    void dispatchDemoOrder();
    void printTracking() const;
    void warehouseDemo();
    void financeDemo();
    void serviceDemo();
    void analyticsDemo() const;

    TransportCompany company_;
    CompanyFinance finance_;
    ServiceManagement service_;
    std::shared_ptr<Client> demoClient_;
    std::shared_ptr<Driver> demoDriver_;
    std::shared_ptr<Vehicle> demoVehicle_;
    std::shared_ptr<Order> demoOrder_;
    std::shared_ptr<Warehouse> demoWarehouse_;
    std::shared_ptr<Cargo> demoCargo_;
    StorageUnit demoUnit_;
};

} // namespace transport::cli
