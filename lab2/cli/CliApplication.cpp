#include "CliApplication.h"

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include "transport/analytics/CustomerFeedback.h"
#include "transport/analytics/FuelConsumption.h"
#include "transport/analytics/Metrics.h"
#include "transport/analytics/PerformanceMetric.h"
#include "transport/analytics/TrafficInfo.h"
#include "transport/cargo/Cargo.h"
#include "transport/core/Address.h"
#include "transport/core/Money.h"
#include "transport/core/TransportDate.h"
#include "transport/finance/BankAccount.h"
#include "transport/finance/Invoice.h"
#include "transport/finance/Payment.h"
#include "transport/finance/TaxCalculator.h"
#include "transport/fleet/Machine.h"
#include "transport/fleet/Truck.h"
#include "transport/orders/Client.h"
#include "transport/orders/Order.h"
#include "transport/orders/Route.h"
#include "transport/orders/RoutePoint.h"
#include "transport/service/LoyaltyProgram.h"
#include "transport/staff/Driver.h"
#include "transport/staff/DrivingLicense.h"
#include "transport/staff/Salary.h"
#include "transport/warehouse/Rack.h"
#include "transport/warehouse/StorageUnit.h"
#include "transport/warehouse/StorageZone.h"
#include "transport/warehouse/Warehouse.h"

namespace transport::cli {
namespace {

constexpr int kExitChoice = 0;
constexpr int kOverviewChoice = 1;
constexpr int kDispatchChoice = 2;
constexpr int kTrackingChoice = 3;
constexpr int kWarehouseChoice = 4;
constexpr int kFinanceChoice = 5;
constexpr int kServiceChoice = 6;
constexpr int kAnalyticsChoice = 7;

constexpr int64_t kDemoClientId = 1;
constexpr int64_t kDemoDriverId = 2;
constexpr int64_t kDemoOrderId = 1;
constexpr double kDemoBalance = 100000.0;
constexpr double kDemoSalary = 90000.0;
constexpr double kDemoCargoWeight = 100.0;
constexpr double kDemoCargoVolume = 2.0;
constexpr double kDemoVehicleCapacityKg = 1000.0;
constexpr double kDemoVehicleCapacityM3 = 20.0;
constexpr double kDemoRatePerKm = 20.0;
constexpr double kVatRate = 0.20;
constexpr double kIncomeTaxRate = 0.13;
constexpr int kDemoLoyaltyPoints = 120;
constexpr int kDemoExperienceYears = 5;
constexpr int kDemoAxles = 3;
constexpr int kDemoRating = 2;
constexpr int kFeedbackRating = 5;

Address demoAddress() {
    return Address("Main street", "Moscow", "101000", "1");
}

TransportDate demoDate() {
    return TransportDate(15, 11, 2026);
}

Salary demoSalary() {
    return Salary(Money(kDemoSalary, "RUB"), Money(0.0, "RUB"), Money(0.0, "RUB"));
}

BankAccount demoAccount() {
    return BankAccount("RU00TRANSPORT000000000000", Money(kDemoBalance, "RUB"));
}

TaxCalculator demoTaxCalculator() {
    return TaxCalculator(kVatRate, kIncomeTaxRate);
}

} // namespace

CliApplication::CliApplication()
    : company_("TransLogistic", demoAddress(), demoAccount(), demoTaxCalculator()),
      finance_(demoAccount(), demoTaxCalculator()),
      service_(LoyaltyProgram(1, 100, 500), CustomerService()) {
    demoClient_ = std::make_shared<Client>(kDemoClientId, "Anna Smirnova", demoAddress(),
                                           "+7-900-000-00-02", Money(50000.0, "RUB"));
    demoClient_->addLoyaltyPoints(kDemoLoyaltyPoints);

    const DrivingLicense license("LIC-1", "B", TransportDate(1, 1, 2020),
                                 TransportDate(1, 1, 2030));
    demoDriver_ = std::make_shared<Driver>(kDemoDriverId, "Petr Sidorov", demoAddress(),
                                           "+7-900-000-00-01", demoSalary(), demoDate(),
                                           license, kDemoExperienceYears);
    demoVehicle_ = std::make_shared<Truck>("A001AA", kDemoVehicleCapacityKg,
                                           kDemoVehicleCapacityM3, 1000.0, kDemoRatePerKm,
                                           Machine("ENG-1", "Diesel", 10.0), kDemoAxles);
    demoCargo_ = std::make_shared<Cargo>(1, "Boxes", kDemoCargoWeight, kDemoCargoVolume,
                                         Money(1000.0, "RUB"));

    company_.hire(demoDriver_);
    company_.addClient(demoClient_);
    company_.addVehicle(demoVehicle_);

    Route route("Moscow - Tver");
    route.addPoint(RoutePoint(demoAddress(), GPSPosition(55.7558, 37.6173), 0, kDemoOrderId));
    route.addPoint(RoutePoint(Address("Sovetskaya", "Tver", "170000", "5"),
                              GPSPosition(56.8587, 35.9176), 1, kDemoOrderId));
    demoOrder_ = std::make_shared<Order>(kDemoOrderId, demoClient_, demoCargo_, route,
                                         demoDate(), TransportDate(20, 11, 2026));
    company_.placeOrder(demoOrder_);

    demoWarehouse_ = std::make_shared<Warehouse>("Central warehouse", demoAddress());
    StorageZone zone("Main zone", 20.0);
    zone.addRack(Rack(1, 3, kDemoVehicleCapacityKg));
    demoWarehouse_->addZone(zone);
    demoUnit_ = StorageUnit("U-1", kDemoVehicleCapacityKg, "Main zone");
    service_.onboardClient(demoClient_);
}

int CliApplication::run() {
    while (true) {
        printMenu();
        const int choice = readChoice();
        switch (choice) {
            case kOverviewChoice:
                printOverview();
                break;
            case kDispatchChoice:
                dispatchDemoOrder();
                break;
            case kTrackingChoice:
                printTracking();
                break;
            case kWarehouseChoice:
                warehouseDemo();
                break;
            case kFinanceChoice:
                financeDemo();
                break;
            case kServiceChoice:
                serviceDemo();
                break;
            case kAnalyticsChoice:
                analyticsDemo();
                break;
            case kExitChoice:
                return 0;
            default:
                std::cout << "Unknown menu option\n";
                break;
        }
    }
}

void CliApplication::printMenu() const {
    std::cout << "\n=== Transport company ===\n"
              << "1. Company overview\n"
              << "2. Dispatch demo order\n"
              << "3. Show order tracking\n"
              << "4. Warehouse operations\n"
              << "5. Finance report\n"
              << "6. Customer service\n"
              << "7. Analytics summary\n"
              << "0. Exit\n"
              << "Choose an option: ";
}

int CliApplication::readChoice() const {
    int choice = 0;
    if (!(std::cin >> choice)) {
        return kExitChoice;
    }
    return choice;
}

void CliApplication::printOverview() const {
    std::cout << "Company: " << company_.name() << "\n"
              << "Employees: " << company_.employees().size() << "\n"
              << "Vehicles: " << company_.fleet().size() << "\n"
              << "Clients: " << company_.clients().size() << "\n"
              << "Orders: " << company_.orders().size() << "\n"
              << "Warehouses: " << company_.warehouses().size() << "\n"
              << "Finance: " << company_.financialReport() << "\n"
              << "Quality: " << company_.qualityReport() << "\n";
}

void CliApplication::dispatchDemoOrder() {
    if (demoOrder_->status() != OrderStatus::Created) {
        std::cout << "Order is no longer waiting for dispatch\n";
        return;
    }
    try {
        company_.dispatchOrder(kDemoOrderId, demoDriver_, demoVehicle_, demoDate());
        std::cout << "Order dispatched. Price: " << demoOrder_->price().format() << "\n";
    } catch (const std::exception& error) {
        std::cout << "Dispatch failed: " << error.what() << "\n";
    }
}

void CliApplication::printTracking() const {
    if (!company_.tracking().hasHistory(kDemoOrderId)) {
        std::cout << "No tracking events yet\n";
        return;
    }
    std::cout << "Tracking history:\n";
    for (const std::string& event : company_.tracking().historyFor(kDemoOrderId)) {
        std::cout << "  " << event << "\n";
    }
    std::cout << "Current status: " << company_.tracking().currentStatus(kDemoOrderId)
              << "\n";
}

void CliApplication::warehouseDemo() {
    try {
        demoWarehouse_->receiveCargo(*demoCargo_, demoUnit_);
        std::cout << "Cargo stored. Inventory: "
                  << demoWarehouse_->inventoryValue().format()
                  << ", unit occupancy: " << demoUnit_.occupancy() << " kg\n";
        demoWarehouse_->shipCargo(*demoCargo_, demoUnit_);
        std::cout << "Cargo shipped. Inventory: "
                  << demoWarehouse_->inventoryValue().format() << "\n";
    } catch (const std::exception& error) {
        std::cout << "Warehouse operation failed: " << error.what() << "\n";
    }
}

void CliApplication::financeDemo() {
    const Money amount = demoOrder_->price().isZero() ? Money(1000.0, "RUB")
                                                       : demoOrder_->price();
    Invoice invoice("INV-CLI", kDemoOrderId, kDemoClientId, amount, demoDate(),
                    TransportDate(1, 12, 2026));
    finance_.addInvoice(invoice);

    Payment payment(invoice.number(), amount, "Card");
    payment.process(demoDate());
    finance_.recordIncome(payment);
    finance_.recordExpense("Fuel", Money(5000.0, "RUB"));
    std::cout << "Finance report: " << finance_.monthlyReport() << "\n";
}

void CliApplication::serviceDemo() {
    try {
        const CustomerFeedback feedback = service_.processComplaint(
            kDemoClientId, kDemoOrderId, kDemoRating, "Late delivery", demoDate());
        std::cout << "Feedback: " << feedback.summary() << "\n"
                  << "Average rating: " << service_.monitor().averageRating() << "\n"
                  << "Loyalty: " << service_.loyaltyReport(*demoClient_) << "\n";
    } catch (const std::exception& error) {
        std::cout << "Customer service operation failed: " << error.what() << "\n";
    }
}

void CliApplication::analyticsDemo() const {
    Metrics metrics(FuelConsumption(200.0, 20.0, 50.0),
                    TrafficInfo("Moscow", 30, 4));
    metrics.addMetric(PerformanceMetric("On-time delivery", 90.0, 80.0));
    metrics.addMetric(PerformanceMetric("Customer rating", kFeedbackRating,
                                        kFeedbackRating));
    std::cout << "Analytics: " << metrics.summarize() << "\n";
}

} // namespace transport::cli
