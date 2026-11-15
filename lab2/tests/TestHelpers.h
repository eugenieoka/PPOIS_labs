#pragma once

#include <memory>
#include <string>

#include "transport/analytics/CustomerFeedback.h"
#include "transport/cargo/Cargo.h"
#include "transport/cargo/FragileCargo.h"
#include "transport/cargo/PerishableCargo.h"
#include "transport/core/Address.h"
#include "transport/core/GPSPosition.h"
#include "transport/core/Money.h"
#include "transport/core/TransportDate.h"
#include "transport/finance/BankAccount.h"
#include "transport/finance/TaxCalculator.h"
#include "transport/fleet/Machine.h"
#include "transport/fleet/Truck.h"
#include "transport/fleet/Van.h"
#include "transport/orders/Client.h"
#include "transport/orders/ExpressDelivery.h"
#include "transport/orders/Order.h"
#include "transport/orders/Route.h"
#include "transport/orders/RoutePoint.h"
#include "transport/staff/Driver.h"
#include "transport/staff/Employee.h"
#include "transport/staff/Salary.h"
#include "transport/warehouse/StorageUnit.h"

namespace transport::test {

inline Address address(std::string street = "Main", std::string city = "Moscow",
                       std::string postalCode = "101000", std::string building = "1") {
    return Address(std::move(street), std::move(city), std::move(postalCode), std::move(building));
}

inline TransportDate date(int day = 1, int month = 1, int year = 2026) {
    return TransportDate(day, month, year);
}

inline Salary salary(double base = 100000.0, double bonus = 0.0, double deductions = 0.0) {
    return Salary(Money(base, "RUB"), Money(bonus, "RUB"), Money(deductions, "RUB"));
}

inline Machine machine(double fuelPerHour = 10.0) {
    return Machine("ENG-1", "Diesel", fuelPerHour);
}

inline Employee employee(int64_t id = 1, const std::string& name = "Ivan Petrov") {
    return Employee(id, name, address(), "+7-900-000-00-00", salary(), date());
}

inline Driver driver(int64_t id = 2, const std::string& name = "Petr Sidorov") {
    const DrivingLicense license("LIC-1", "B", date(1, 1, 2020), date(1, 1, 2030));
    return Driver(id, name, address(), "+7-900-000-00-01", salary(), date(), license, 5);
}

inline Client client(int64_t id = 1, double balance = 10000.0) {
    return Client(id, "Anna Smirnova", address(), "+7-900-000-00-02", Money(balance, "RUB"));
}

inline std::shared_ptr<Client> clientPtr(int64_t id = 1, double balance = 10000.0) {
    return std::make_shared<Client>(client(id, balance));
}

inline std::shared_ptr<Cargo> cargo(int64_t id = 1, double weight = 100.0) {
    return std::make_shared<Cargo>(id, "Boxes", weight, 2.0, Money(1000.0, "RUB"));
}

inline std::shared_ptr<FragileCargo> fragileCargo(int64_t id = 2, double weight = 50.0) {
    return std::make_shared<FragileCargo>(id, "Glass", weight, 1.0, Money(2000.0, "RUB"),
                                          "Crate", 30);
}

inline std::shared_ptr<PerishableCargo> perishableCargo(int64_t id = 3, double weight = 40.0) {
    return std::make_shared<PerishableCargo>(id, "Milk", weight, 0.5, Money(500.0, "RUB"),
                                             date(10, 1, 2026), 2.0, 6.0);
}

inline RoutePoint routePoint(int sequence, int64_t orderId = 1,
                             double latitude = 55.75, double longitude = 37.61) {
    return RoutePoint(address(), GPSPosition(latitude, longitude), sequence, orderId);
}

inline Route route(int pointCount = 3, int64_t orderId = 1) {
    Route result("Main route");
    for (int i = 0; i < pointCount; ++i) {
        result.addPoint(routePoint(i, orderId, 55.75 + i * 0.01, 37.61 + i * 0.01));
    }
    return result;
}

inline Truck truck(const std::string& plate = "A001AA", double capacityKg = 1000.0) {
    return Truck(plate, capacityKg, 20.0, 1000.0, 20.0, machine(), 3);
}

inline Van van(const std::string& plate = "B002BB", bool refrigerated = true) {
    return Van(plate, 500.0, 10.0, 500.0, 15.0, machine(), refrigerated, 2);
}

inline std::shared_ptr<Order> order(int64_t id = 1, std::shared_ptr<Client> clientPtr = nullptr,
                                    std::shared_ptr<Cargo> cargoPtr = nullptr,
                                    const Route& routeValue = route()) {
    if (!clientPtr) {
        clientPtr = std::make_shared<Client>(client());
    }
    if (!cargoPtr) {
        cargoPtr = cargo();
    }
    return std::make_shared<Order>(id, std::move(clientPtr), std::move(cargoPtr),
                                   routeValue, date(), date(10, 1, 2026));
}

inline std::shared_ptr<ExpressDelivery> expressOrder(int64_t id = 2,
                                                     std::shared_ptr<Client> clientPtr = nullptr,
                                                     std::shared_ptr<Cargo> cargoPtr = nullptr) {
    if (!clientPtr) {
        clientPtr = std::make_shared<Client>(client());
    }
    if (!cargoPtr) {
        cargoPtr = cargo();
    }
    return std::make_shared<ExpressDelivery>(id, std::move(clientPtr), std::move(cargoPtr),
                                             route(), date(), date(10, 1, 2026), 24, 0.25);
}

inline BankAccount bankAccount(double balance = 100000.0) {
    return BankAccount("RU00TEST000000000000000000", Money(balance, "RUB"));
}

inline TaxCalculator taxCalculator() {
    return TaxCalculator(0.20, 0.13);
}

inline CustomerFeedback feedback(int rating = 5, const std::string& comment = "Great") {
    return CustomerFeedback(1, 1, rating, comment, date());
}

inline std::shared_ptr<StorageUnit> storageUnit(double capacity = 10000.0) {
    return std::make_shared<StorageUnit>("U-1", capacity, "Zone A");
}

} // namespace transport::test
