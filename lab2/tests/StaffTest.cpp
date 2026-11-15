#include "TestHelpers.h"

#include <gtest/gtest.h>

#include <cmath>
#include <limits>

#include "transport/exceptions/DriverNotAvailableException.h"
#include "transport/exceptions/InsufficientFundsException.h"
#include "transport/exceptions/InvalidLicenseException.h"
#include "transport/exceptions/InvalidMoneyException.h"
#include "transport/exceptions/InvalidOrderStateException.h"
#include "transport/exceptions/InvalidRatingException.h"
#include "transport/exceptions/TransportException.h"
#include "transport/finance/BankAccount.h"
#include "transport/finance/Invoice.h"
#include "transport/finance/Payment.h"
#include "transport/fleet/MaintenanceRecord.h"
#include "transport/fleet/Truck.h"
#include "transport/orders/Order.h"
#include "transport/staff/Accountant.h"
#include "transport/staff/Dispatcher.h"
#include "transport/staff/DrivingLicense.h"
#include "transport/staff/Employee.h"
#include "transport/staff/HRManager.h"
#include "transport/staff/Manager.h"
#include "transport/staff/Mechanic.h"
#include "transport/staff/Salary.h"
#include "transport/staff/WarehouseManager.h"
#include "transport/warehouse/StorageUnit.h"
#include "transport/warehouse/Warehouse.h"

namespace {

using namespace transport;
using namespace transport::test;

TEST(SalaryTest, ComputesNetAndMutatesComponents) {
    Salary salaryValue(Money(1000.0, "RUB"), Money(100.0, "RUB"), Money(50.0, "RUB"));
    EXPECT_NEAR(salaryValue.net().amount(), 1050.0, 1e-9);
    EXPECT_TRUE(salaryValue.isPositive());

    salaryValue.addBonus(Money(20.0, "RUB"));
    EXPECT_NEAR(salaryValue.net().amount(), 1070.0, 1e-9);

    salaryValue.addDeduction(Money(70.0, "RUB"));
    EXPECT_NEAR(salaryValue.net().amount(), 1000.0, 1e-9);
}

TEST(SalaryTest, RejectsInvalidComponents) {
    EXPECT_THROW(Salary(Money(-1.0, "RUB"), Money(0.0, "RUB"), Money(0.0, "RUB")),
                 InvalidMoneyException);
    EXPECT_THROW(Salary(Money(100.0, "RUB"), Money(0.0, "RUB"), Money(101.0, "RUB")),
                 InvalidMoneyException);

    Salary salaryValue(Money(100.0, "RUB"), Money(0.0, "RUB"), Money(0.0, "RUB"));
    EXPECT_THROW(salaryValue.addBonus(Money(-1.0, "RUB")), InvalidMoneyException);
    EXPECT_THROW(salaryValue.addDeduction(Money(-1.0, "RUB")), InvalidMoneyException);
    EXPECT_THROW(salaryValue.addDeduction(Money(101.0, "RUB")), InvalidMoneyException);
}

TEST(DrivingLicenseTest, ValidatesDatesAndCategories) {
    const DrivingLicense license("LIC-1", "E", date(1, 1, 2020), date(1, 1, 2030));
    EXPECT_TRUE(license.isValidOn(date(1, 1, 2025)));
    EXPECT_FALSE(license.isValidOn(date(1, 1, 2019)));
    EXPECT_FALSE(license.isValidOn(date(1, 1, 2031)));
    EXPECT_TRUE(license.coversCategory("B"));
    EXPECT_TRUE(license.coversCategory("E"));
    EXPECT_TRUE(license.coversCategory("D"));

    const DrivingLicense bLicense("LIC-2", "B", date(1, 1, 2020), date(1, 1, 2030));
    EXPECT_TRUE(bLicense.coversCategory("B"));
    EXPECT_FALSE(bLicense.coversCategory("C"));
}

TEST(DrivingLicenseTest, RejectsInvalidDataAndRenews) {
    EXPECT_THROW(DrivingLicense("", "B", date(), date(1, 1, 2030)), InvalidLicenseException);
    EXPECT_THROW(DrivingLicense("LIC", "X", date(), date(1, 1, 2030)), InvalidLicenseException);
    EXPECT_THROW(DrivingLicense("LIC", "B", date(1, 1, 2030), date()), InvalidLicenseException);

    DrivingLicense license("LIC", "B", date(), date(1, 1, 2030));
    license.renew(date(1, 1, 2031));
    EXPECT_EQ(license.expiresOn().year(), 2031);
    EXPECT_THROW(license.renew(date(1, 1, 2030)), InvalidLicenseException);
}

TEST(EmployeeTest, ComputesCostTenureAndRaise) {
    Employee employeeValue = employee();
    EXPECT_EQ(employeeValue.roleName(), "Employee");
    EXPECT_NEAR(employeeValue.annualCost().amount(), 1200000.0, 1e-9);
    EXPECT_EQ(employeeValue.tenureYears(date(1, 1, 2027)), 1);
    EXPECT_EQ(employeeValue.tenureYears(date(1, 1, 2020)), 0);

    employeeValue.giveRaise(10.0);
    EXPECT_NEAR(employeeValue.salary().base().amount(), 110000.0, 1e-9);

    employeeValue.deactivate();
    EXPECT_FALSE(employeeValue.isActive());
    employeeValue.activate();
    EXPECT_TRUE(employeeValue.isActive());
}

TEST(EmployeeTest, RejectsInvalidDataAndRaise) {
    EXPECT_THROW(Employee(0, "Name", address(), "phone", salary(), date()), TransportException);
    EXPECT_THROW(Employee(1, "", address(), "phone", salary(), date()), TransportException);
    EXPECT_THROW(Employee(1, "Name", address(), "", salary(), date()), TransportException);

    Employee employeeValue = employee();
    EXPECT_THROW(employeeValue.giveRaise(-100.0), TransportException);
    EXPECT_THROW(employeeValue.giveRaise(1001.0), TransportException);
    EXPECT_THROW(employeeValue.giveRaise(std::numeric_limits<double>::quiet_NaN()),
                 TransportException);
}

TEST(DriverTest, AssignsAndCompletesDeliveries) {
    Driver driverValue = driver();
    EXPECT_EQ(driverValue.roleName(), "Driver");
    EXPECT_TRUE(driverValue.isAvailable());
    EXPECT_TRUE(driverValue.canDrive("B"));
    EXPECT_FALSE(driverValue.canDrive("C"));

    driverValue.assign();
    EXPECT_FALSE(driverValue.isAvailable());
    EXPECT_THROW(driverValue.assign(), DriverNotAvailableException);

    driverValue.completeDelivery();
    EXPECT_TRUE(driverValue.isAvailable());
    EXPECT_EQ(driverValue.completedDeliveries(), 1);
    EXPECT_THROW(driverValue.completeDelivery(), InvalidOrderStateException);

    driverValue.updateRating(4.5);
    EXPECT_DOUBLE_EQ(driverValue.rating(), 4.5);
    EXPECT_THROW(driverValue.updateRating(6.0), InvalidRatingException);
}

TEST(DriverTest, RejectsInvalidExperience) {
    const DrivingLicense license("LIC", "B", date(), date(1, 1, 2030));
    EXPECT_THROW(Driver(1, "Name", address(), "phone", salary(), date(), license, -1),
                 TransportException);
    EXPECT_THROW(Driver(1, "Name", address(), "phone", salary(), date(), license, 61),
                 TransportException);
}

TEST(DispatcherTest, ManagesOrderCapacityAndShift) {
    Dispatcher dispatcherValue(1, "Name", address(), "phone", salary(), date(), "Moscow", 2, "Day");
    EXPECT_EQ(dispatcherValue.roleName(), "Dispatcher");
    EXPECT_TRUE(dispatcherValue.canHandleMore());

    dispatcherValue.assignOrder();
    dispatcherValue.assignOrder();
    EXPECT_FALSE(dispatcherValue.canHandleMore());
    EXPECT_EQ(dispatcherValue.activeOrders(), 2);
    EXPECT_THROW(dispatcherValue.assignOrder(), TransportException);
    EXPECT_THROW(dispatcherValue.closeShift(), InvalidOrderStateException);

    dispatcherValue.releaseOrder();
    dispatcherValue.releaseOrder();
    EXPECT_THROW(dispatcherValue.releaseOrder(), InvalidOrderStateException);
    dispatcherValue.closeShift();
    EXPECT_EQ(dispatcherValue.shift(), "Closed");
}

TEST(DispatcherTest, RejectsInvalidConstruction) {
    EXPECT_THROW(Dispatcher(1, "Name", address(), "phone", salary(), date(), "", 1, "Day"),
                 TransportException);
    EXPECT_THROW(Dispatcher(1, "Name", address(), "phone", salary(), date(), "Moscow", 1, ""),
                 TransportException);
    EXPECT_THROW(Dispatcher(1, "Name", address(), "phone", salary(), date(), "Moscow", 0, "Day"),
                 TransportException);
}

TEST(ManagerTest, ApprovesExpensesAndIncreasesBudget) {
    Manager managerValue(1, "Name", address(), "phone", salary(), date(), "Sales",
                         Money(1000.0, "RUB"));
    EXPECT_EQ(managerValue.roleName(), "Manager");
    EXPECT_NEAR(managerValue.remainingBudget().amount(), 1000.0, 1e-9);

    managerValue.approveExpense(Money(200.0, "RUB"));
    EXPECT_NEAR(managerValue.remainingBudget().amount(), 800.0, 1e-9);
    managerValue.increaseBudget(Money(100.0, "RUB"));
    EXPECT_NEAR(managerValue.remainingBudget().amount(), 900.0, 1e-9);

    EXPECT_THROW(managerValue.approveExpense(Money(-1.0, "RUB")), InvalidMoneyException);
    EXPECT_THROW(managerValue.approveExpense(Money(1000.0, "RUB")), InsufficientFundsException);
    EXPECT_THROW(managerValue.increaseBudget(Money(-1.0, "RUB")), InvalidMoneyException);
}

TEST(ManagerTest, RejectsInvalidConstruction) {
    EXPECT_THROW(Manager(1, "Name", address(), "phone", salary(), date(), "",
                         Money(1.0, "RUB")), TransportException);
    EXPECT_THROW(Manager(1, "Name", address(), "phone", salary(), date(), "Sales",
                         Money(-1.0, "RUB")), InvalidMoneyException);
}

TEST(HRManagerTest, HiresAndFiresEmployees) {
    HRManager managerValue(1, "Name", address(), "phone", salary(), date(), "HR",
                           Money(1000.0, "RUB"), 1);
    Employee candidate = employee(2);
    candidate.deactivate();

    EXPECT_EQ(managerValue.roleName(), "HRManager");
    managerValue.hire(candidate);
    EXPECT_TRUE(candidate.isActive());
    EXPECT_EQ(managerValue.hiredCount(), 1);
    EXPECT_EQ(managerValue.openPositions(), 0);
    EXPECT_THROW(managerValue.hire(candidate), TransportException);

    managerValue.fire(candidate);
    EXPECT_FALSE(candidate.isActive());
    EXPECT_EQ(managerValue.openPositions(), 1);
    EXPECT_THROW(managerValue.fire(candidate), TransportException);

    managerValue.closePosition();
    EXPECT_THROW(managerValue.closePosition(), TransportException);
    managerValue.openPosition();
    EXPECT_EQ(managerValue.openPositions(), 1);
}

TEST(HRManagerTest, RejectsInvalidStateAndPositions) {
    EXPECT_THROW(HRManager(1, "Name", address(), "phone", salary(), date(), "HR",
                           Money(1.0, "RUB"), -1), TransportException);
    HRManager managerValue(1, "Name", address(), "phone", salary(), date(), "HR",
                           Money(1.0, "RUB"), 0);
    Employee candidate = employee(2);
    EXPECT_THROW(managerValue.hire(candidate), TransportException);

    HRManager openManager(2, "Name", address(), "phone", salary(), date(), "HR",
                          Money(1.0, "RUB"), 1);
    Employee activeCandidate = employee(3);
    EXPECT_THROW(openManager.hire(activeCandidate), TransportException);
}

TEST(WarehouseManagerTest, ReceivesAndReleasesCargo) {
    auto warehouse = std::make_shared<Warehouse>("Central", address());
    WarehouseManager managerValue(1, "Name", address(), "phone", salary(), date(), "Warehouse",
                                  Money(1000.0, "RUB"), warehouse);
    StorageUnit unit("U-1", 1000.0, "Zone A");
    const auto cargoValue = cargo(1, 100.0);

    EXPECT_EQ(managerValue.roleName(), "WarehouseManager");
    managerValue.receiveCargo(*cargoValue, unit);
    EXPECT_NEAR(managerValue.inventoryValue().amount(), 1000.0, 1e-9);
    EXPECT_NEAR(unit.occupancy(), 100.0, 1e-9);

    managerValue.releaseCargo(*cargoValue, unit);
    EXPECT_NEAR(managerValue.inventoryValue().amount(), 0.0, 1e-9);
    EXPECT_NEAR(unit.occupancy(), 0.0, 1e-9);

    EXPECT_NE(managerValue.capacityReport().find("Central"), std::string::npos);
}

TEST(WarehouseManagerTest, RejectsNullWarehouse) {
    EXPECT_THROW(WarehouseManager(1, "Name", address(), "phone", salary(), date(), "Warehouse",
                                  Money(1.0, "RUB"), nullptr), TransportException);
}

TEST(AccountantTest, IssuesInvoicesAndProcessesPayments) {
    Accountant accountantValue(1, "Name", address(), "phone", salary(), date(), taxCalculator());
    const auto orderValue = order();
    orderValue->calculatePrice();

    const Invoice invoice = accountantValue.issueInvoice(*orderValue, *orderValue->client(),
                                                         date(), date(10, 1, 2026));
    EXPECT_EQ(invoice.orderId(), 1);
    EXPECT_EQ(invoice.clientId(), 1);
    EXPECT_EQ(accountantValue.processedInvoices(), 1);
    EXPECT_EQ(accountantValue.roleName(), "Accountant");

    Payment payment(invoice.number(), invoice.amount(), "Card");
    BankAccount account = bankAccount(0.0);
    accountantValue.processPayment(payment, account, date());
    EXPECT_TRUE(payment.isSuccessful());
    EXPECT_NEAR(account.balance().amount(), invoice.amount().amount(), 1e-9);

    EXPECT_NEAR(accountantValue.calculateSalary(accountantValue).amount(), 100000.0, 1e-9);
    EXPECT_NEAR(accountantValue.taxReport(Money(1000.0, "RUB")).amount(), 130.0, 1e-9);
}

TEST(MechanicTest, RepairsMatchingVehicles) {
    Mechanic mechanicValue(1, "Name", address(), "phone", salary(), date(), "Universal");
    Truck truckValue = truck();
    const MaintenanceRecord record = mechanicValue.performMaintenance(
        truckValue, date(), Money(500.0, "RUB"));

    EXPECT_EQ(mechanicValue.roleName(), "Mechanic");
    EXPECT_TRUE(mechanicValue.canRepair(truckValue));
    EXPECT_EQ(mechanicValue.repairsCompleted(), 1);
    EXPECT_EQ(truckValue.status(), VehicleStatus::Maintenance);
    EXPECT_NE(record.summary().find("A001AA"), std::string::npos);

    mechanicValue.completeRepair(truckValue);
    EXPECT_EQ(truckValue.status(), VehicleStatus::Available);
}

TEST(MechanicTest, RejectsMismatchedVehicle) {
    Mechanic mechanicValue(1, "Name", address(), "phone", salary(), date(), "Van");
    Truck truckValue = truck();
    EXPECT_FALSE(mechanicValue.canRepair(truckValue));
    EXPECT_THROW(mechanicValue.performMaintenance(truckValue, date(), Money(1.0, "RUB")),
                 TransportException);
    EXPECT_THROW(Mechanic(1, "Name", address(), "phone", salary(), date(), ""),
                 TransportException);
}

} // namespace
