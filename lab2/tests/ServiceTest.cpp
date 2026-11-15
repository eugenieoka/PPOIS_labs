#include "TestHelpers.h"

#include <gtest/gtest.h>

#include "transport/exceptions/InvalidClientDataException.h"
#include "transport/exceptions/InvalidMoneyException.h"
#include "transport/exceptions/OrderNotFoundException.h"
#include "transport/exceptions/TransportException.h"
#include "transport/fleet/Truck.h"
#include "transport/orders/Client.h"
#include "transport/orders/Order.h"
#include "transport/service/CustomerService.h"
#include "transport/service/LoyaltyProgram.h"
#include "transport/service/ServiceManagement.h"
#include "transport/service/TransportCompany.h"
#include "transport/staff/Employee.h"
#include "transport/warehouse/Warehouse.h"

namespace {

using namespace transport;
using namespace transport::test;

TEST(LoyaltyProgramTest, EarnsRedeemsAndAssignsTiers) {
    LoyaltyProgram program(2, 100, 500);
    EXPECT_EQ(program.earn(Money(50.5, "RUB")), 100);
    EXPECT_EQ(program.tierFor(0), "Standard");
    EXPECT_EQ(program.tierFor(100), "Silver");
    EXPECT_EQ(program.tierFor(500), "Gold");

    Client clientValue = client();
    clientValue.addLoyaltyPoints(100);
    EXPECT_TRUE(program.canRedeem(clientValue, 50));
    EXPECT_FALSE(program.canRedeem(clientValue, 101));
    program.redeem(clientValue, 50);
    EXPECT_EQ(clientValue.loyaltyPoints(), 50);
    EXPECT_THROW(program.redeem(clientValue, 100), InvalidClientDataException);
    EXPECT_THROW((void)program.earn(Money(-1.0, "RUB")), InvalidMoneyException);
}

TEST(LoyaltyProgramTest, RejectsInvalidConfiguration) {
    EXPECT_THROW(LoyaltyProgram(0, 100, 500), TransportException);
    EXPECT_THROW(LoyaltyProgram(1, 0, 500), TransportException);
    EXPECT_THROW(LoyaltyProgram(1, 500, 100), TransportException);
}

TEST(CustomerServiceTest, RegistersClientsAndFilesComplaints) {
    CustomerService service;
    const auto clientValue = clientPtr();
    service.registerClient(clientValue);
    EXPECT_EQ(service.clients().size(), 1);
    EXPECT_EQ(service.findClient(1), clientValue);

    const CustomerFeedback feedback = service.fileComplaint(1, 1, 2, "Late", date());
    EXPECT_EQ(feedback.rating(), 2);
    EXPECT_EQ(service.monitor().feedbackCount(), 1);
    EXPECT_EQ(service.monitor().topComplaints(1)[0], "Late");

    EXPECT_THROW(service.registerClient(nullptr), TransportException);
    EXPECT_THROW(service.findClient(99), OrderNotFoundException);
    EXPECT_THROW(service.fileComplaint(99, 1, 5, "Great", date()), OrderNotFoundException);
}

TEST(ServiceManagementTest, OnboardsAndProcessesComplaints) {
    ServiceManagement management(LoyaltyProgram(1, 100, 500), CustomerService());
    const auto clientValue = clientPtr();
    management.onboardClient(clientValue);
    clientValue->addLoyaltyPoints(100);

    const CustomerFeedback feedback = management.processComplaint(1, 1, 2, "Late", date());
    EXPECT_EQ(feedback.rating(), 2);
    EXPECT_EQ(management.monitor().feedbackCount(), 1);
    EXPECT_NE(management.loyaltyReport(*clientValue).find("Silver"), std::string::npos);
}

TEST(TransportCompanyTest, ManagesResourcesAndOrders) {
    TransportCompany company("Fast", address(), bankAccount(), taxCalculator());
    EXPECT_EQ(company.name(), "Fast");

    company.hire(std::make_shared<Employee>(employee()));
    company.addClient(clientPtr());
    const auto vehicle = std::make_shared<Truck>(truck());
    company.addVehicle(vehicle);
    company.addWarehouse(Warehouse("Central", address()));
    EXPECT_EQ(company.employees().size(), 1);
    EXPECT_EQ(company.fleet().size(), 1);
    EXPECT_EQ(company.clients().size(), 1);
    EXPECT_EQ(company.warehouses().size(), 1);

    const auto orderValue = order();
    company.placeOrder(orderValue);
    EXPECT_EQ(company.orders().size(), 1);
    EXPECT_EQ(company.findOrder(1), orderValue);
    EXPECT_EQ(company.findClient(1), company.clients().front());

    const auto driverValue = std::make_shared<Driver>(driver());
    company.dispatchOrder(1, driverValue, vehicle, date(1, 1, 2026));
    EXPECT_EQ(orderValue->status(), OrderStatus::Assigned);
    EXPECT_TRUE(orderValue->price().amount() > 0.0);
    EXPECT_TRUE(company.tracking().hasHistory(1));
    EXPECT_EQ(company.tracking().currentStatus(1), "Assigned");

    company.cancelOrder(1, date(2, 1, 2026));
    EXPECT_EQ(orderValue->status(), OrderStatus::Cancelled);
    EXPECT_EQ(company.tracking().currentStatus(1), "Cancelled");

    EXPECT_NE(company.financialReport().find("balance"), std::string::npos);
    EXPECT_NE(company.qualityReport().find("feedback"), std::string::npos);
}

TEST(TransportCompanyTest, RejectsInvalidAndMissingResources) {
    EXPECT_THROW(TransportCompany("", address(), bankAccount(), taxCalculator()),
                 TransportException);
    TransportCompany company("Fast", address(), bankAccount(), taxCalculator());

    EXPECT_THROW(company.hire(nullptr), TransportException);
    EXPECT_THROW(company.addClient(nullptr), TransportException);
    EXPECT_THROW(company.addVehicle(nullptr), TransportException);
    EXPECT_THROW(company.placeOrder(nullptr), TransportException);

    EXPECT_THROW(company.findOrder(99), OrderNotFoundException);
    EXPECT_THROW(company.findClient(99), OrderNotFoundException);
    EXPECT_THROW(company.dispatchOrder(1, nullptr, nullptr, date()), TransportException);

    const auto orderValue = order();
    company.placeOrder(orderValue);
    const auto driverValue = std::make_shared<Driver>(driver());
    const auto vehicle = std::make_shared<Truck>(truck());
    EXPECT_THROW(company.dispatchOrder(1, driverValue, nullptr, date()), TransportException);
    EXPECT_THROW(company.dispatchOrder(1, nullptr, vehicle, date()), TransportException);
    EXPECT_THROW(company.dispatchOrder(99, driverValue, vehicle, date()),
                 OrderNotFoundException);
}

} // namespace
