#include "TestHelpers.h"

#include <gtest/gtest.h>

#include "transport/exceptions/DriverNotAvailableException.h"
#include "transport/exceptions/InsufficientFundsException.h"
#include "transport/exceptions/InvalidClientDataException.h"
#include "transport/exceptions/InvalidMoneyException.h"
#include "transport/exceptions/InvalidOrderStateException.h"
#include "transport/exceptions/OrderNotFoundException.h"
#include "transport/exceptions/TransportException.h"
#include "transport/exceptions/VehicleNotAvailableException.h"
#include "transport/exceptions/VehicleOverloadException.h"
#include "transport/fleet/Truck.h"
#include "transport/orders/Client.h"
#include "transport/orders/Delivery.h"
#include "transport/orders/ExpressDelivery.h"
#include "transport/orders/LogisticsManager.h"
#include "transport/orders/Order.h"
#include "transport/orders/Route.h"
#include "transport/orders/RoutePoint.h"
#include "transport/orders/TrackingSystem.h"
#include "transport/staff/Dispatcher.h"
#include "transport/staff/Driver.h"

namespace {

using namespace transport;
using namespace transport::test;

TEST(ClientTest, ManagesBalanceLoyaltyAndBlocking) {
    Client clientValue = client(1, 1000.0);
    EXPECT_TRUE(clientValue.canPay(Money(500.0, "RUB")));
    EXPECT_FALSE(clientValue.canPay(Money(1500.0, "RUB")));

    clientValue.deposit(Money(500.0, "RUB"));
    EXPECT_NEAR(clientValue.balance().amount(), 1500.0, 1e-9);
    clientValue.withdraw(Money(200.0, "RUB"));
    EXPECT_NEAR(clientValue.balance().amount(), 1300.0, 1e-9);

    clientValue.addLoyaltyPoints(100);
    EXPECT_NEAR(clientValue.discountRate(), 0.05, 1e-9);
    clientValue.addLoyaltyPoints(400);
    EXPECT_NEAR(clientValue.discountRate(), 0.10, 1e-9);
    clientValue.spendLoyaltyPoints(500);
    EXPECT_EQ(clientValue.loyaltyPoints(), 0);

    clientValue.block();
    EXPECT_TRUE(clientValue.isBlacklisted());
    EXPECT_FALSE(clientValue.canPay(Money(1.0, "RUB")));
    EXPECT_THROW(clientValue.withdraw(Money(1.0, "RUB")), InsufficientFundsException);
    clientValue.unblock();
    EXPECT_FALSE(clientValue.isBlacklisted());
}

TEST(ClientTest, RejectsInvalidOperations) {
    EXPECT_THROW(Client(0, "Name", address(), "phone"), InvalidClientDataException);
    EXPECT_THROW(Client(1, "", address(), "phone"), InvalidClientDataException);
    EXPECT_THROW(Client(1, "Name", address(), ""), InvalidClientDataException);
    EXPECT_THROW(Client(1, "Name", address(), "phone", Money(-1.0, "RUB")),
                 InvalidClientDataException);

    Client clientValue = client();
    EXPECT_THROW(clientValue.deposit(Money(-1.0, "RUB")), InvalidMoneyException);
    EXPECT_THROW(clientValue.withdraw(Money(-1.0, "RUB")), InvalidMoneyException);
    EXPECT_THROW(clientValue.withdraw(Money(100000.0, "RUB")), InsufficientFundsException);
    EXPECT_THROW((void)clientValue.canPay(Money(-1.0, "RUB")), InvalidMoneyException);
    EXPECT_THROW(clientValue.addLoyaltyPoints(-1), InvalidClientDataException);
    EXPECT_THROW(clientValue.spendLoyaltyPoints(1), InvalidClientDataException);
}

TEST(RoutePointTest, ComparesAndDescribes) {
    const RoutePoint first = routePoint(1);
    const RoutePoint same = routePoint(1);
    const RoutePoint otherSequence = routePoint(2);

    EXPECT_TRUE(first.sameAs(same));
    EXPECT_FALSE(first.sameAs(otherSequence));
    EXPECT_NEAR(first.distanceTo(routePoint(2, 1, 55.76, 37.62)), 1.276, 0.05);
    EXPECT_NE(first.describe().find("1."), std::string::npos);
}

TEST(RoutePointTest, RejectsInvalidData) {
    EXPECT_THROW(RoutePoint(address(), GPSPosition(), -1, 1), TransportException);
    EXPECT_THROW(RoutePoint(address(), GPSPosition(), 0, 0), TransportException);
}

TEST(RouteTest, AccumulatesDistanceAndOptimizes) {
    Route routeValue = route(3);
    EXPECT_GT(routeValue.totalDistance(), 0.0);
    EXPECT_GT(routeValue.estimatedDuration(50.0), 0.0);
    EXPECT_TRUE(routeValue.contains(address()));
    EXPECT_THROW((void)routeValue.estimatedDuration(0.0), TransportException);

    Route unsorted("Unsorted");
    unsorted.addPoint(routePoint(2, 1, 55.77, 37.63));
    unsorted.addPoint(routePoint(1, 1, 55.75, 37.61));
    unsorted.optimize();
    ASSERT_EQ(unsorted.points().size(), 2);
    EXPECT_EQ(unsorted.points()[0].sequence(), 1);
    EXPECT_THROW(Route(""), TransportException);
}

TEST(OrderTest, CalculatesPriceAndAssignsResources) {
    const auto clientValue = clientPtr(1, 1000.0);
    const auto orderValue = order(1, clientValue, cargo(1, 100.0));
    const Money price = orderValue->calculatePrice();
    EXPECT_GT(price.amount(), 0.0);
    EXPECT_EQ(orderValue->price(), price);

    const auto driverPtr = std::make_shared<Driver>(driver());
    orderValue->assignDriver(*driverPtr);
    EXPECT_EQ(orderValue->status(), OrderStatus::Assigned);
    EXPECT_TRUE(orderValue->driverId().has_value());
    EXPECT_FALSE(orderValue->isDeliverable());

    const auto vehicle = std::make_shared<Truck>(truck());
    orderValue->assignVehicle(*vehicle);
    EXPECT_TRUE(orderValue->vehiclePlate().has_value());
    EXPECT_TRUE(orderValue->isDeliverable());
}

TEST(OrderTest, RejectsInvalidAssignmentsAndTransitions) {
    const auto orderValue = order();
    const auto driverPtr = std::make_shared<Driver>(driver());
    driverPtr->assign();
    EXPECT_THROW(orderValue->assignDriver(*driverPtr), DriverNotAvailableException);

    const auto vehicle = std::make_shared<Truck>(truck());
    vehicle->changeStatus(VehicleStatus::Maintenance);
    EXPECT_THROW(orderValue->assignVehicle(*vehicle), VehicleNotAvailableException);

    const auto heavyOrder = order(2, clientPtr(), cargo(2, 2000.0));
    const auto smallTruck = std::make_shared<Truck>(truck("A", 1000.0));
    EXPECT_THROW(heavyOrder->assignVehicle(*smallTruck), VehicleOverloadException);

    EXPECT_THROW(orderValue->changeStatus(OrderStatus::InTransit), InvalidOrderStateException);
    orderValue->changeStatus(OrderStatus::Assigned);
    orderValue->changeStatus(OrderStatus::InTransit);
    orderValue->changeStatus(OrderStatus::Delivered);
    EXPECT_THROW(orderValue->changeStatus(OrderStatus::Cancelled), InvalidOrderStateException);
}

TEST(OrderTest, RejectsAssignmentsAfterTransit) {
    const auto orderValue = order();
    orderValue->changeStatus(OrderStatus::Assigned);
    orderValue->changeStatus(OrderStatus::InTransit);

    const Driver driverValue = driver();
    const Truck truckValue = truck();
    EXPECT_THROW(orderValue->assignDriver(driverValue), InvalidOrderStateException);
    EXPECT_THROW(orderValue->assignVehicle(truckValue), InvalidOrderStateException);
}

TEST(OrderTest, CancelsAndChecksOverdue) {
    const auto orderValue = order();
    EXPECT_FALSE(orderValue->isOverdue(date(9, 1, 2026)));
    EXPECT_TRUE(orderValue->isOverdue(date(11, 1, 2026)));
    EXPECT_FALSE(orderValue->isDeliverable());

    orderValue->cancel();
    EXPECT_EQ(orderValue->status(), OrderStatus::Cancelled);
    EXPECT_FALSE(orderValue->isOverdue(date(11, 1, 2026)));
    EXPECT_THROW(orderValue->calculatePrice(), InvalidOrderStateException);
    EXPECT_THROW(orderValue->cancel(), InvalidOrderStateException);
}

TEST(OrderTest, RejectsInvalidConstruction) {
    EXPECT_THROW(Order(0, clientPtr(), cargo(), route(), date(), date(10, 1, 2026)),
                 TransportException);
    EXPECT_THROW(Order(1, nullptr, cargo(), route(), date(), date(10, 1, 2026)),
                 TransportException);
    EXPECT_THROW(Order(1, clientPtr(), nullptr, route(), date(), date(10, 1, 2026)),
                 TransportException);
    EXPECT_THROW(Order(1, clientPtr(), cargo(), route(), date(10, 1, 2026), date()),
                 TransportException);
}

TEST(ExpressDeliveryTest, AddsSurchargeAndEstimatesDate) {
    const auto express = expressOrder();
    const Money price = express->calculatePrice();
    EXPECT_GT(price.amount(), 0.0);
    EXPECT_TRUE(express->isExpress());
    EXPECT_TRUE(express->estimatedDeliveryDate().sameDay(date(2, 1, 2026)));
    EXPECT_EQ(express->guaranteedHours(), 24);
}

TEST(ExpressDeliveryTest, RejectsInvalidTerms) {
    EXPECT_THROW(ExpressDelivery(1, clientPtr(), cargo(), route(), date(),
                                 date(10, 1, 2026), 0, 0.1), TransportException);
    EXPECT_THROW(ExpressDelivery(1, clientPtr(), cargo(), route(), date(),
                                 date(10, 1, 2026), 24, -0.1), TransportException);
}

TEST(DeliveryTest, StartsAndCompletesDelivery) {
    const auto clientValue = clientPtr();
    const auto orderValue = order(1, clientValue, cargo());
    const auto driverPtr = std::make_shared<Driver>(driver());
    const auto vehicle = std::make_shared<Truck>(truck());
    orderValue->assignDriver(*driverPtr);
    orderValue->assignVehicle(*vehicle);

    Delivery delivery(1, orderValue, driverPtr, vehicle);
    EXPECT_EQ(delivery.status(), DeliveryStatus::Planned);
    EXPECT_FALSE(delivery.isOnTime());
    EXPECT_THROW((void)delivery.durationHours(), InvalidOrderStateException);
    EXPECT_THROW(delivery.complete(date(2, 1, 2026)), InvalidOrderStateException);

    delivery.start(date(1, 1, 2026));
    EXPECT_EQ(delivery.status(), DeliveryStatus::InProgress);
    EXPECT_FALSE(driverPtr->isAvailable());
    EXPECT_EQ(vehicle->status(), VehicleStatus::OnRoute);
    EXPECT_EQ(orderValue->status(), OrderStatus::InTransit);
    EXPECT_THROW(delivery.start(date(1, 1, 2026)), InvalidOrderStateException);

    delivery.complete(date(2, 1, 2026));
    EXPECT_EQ(delivery.status(), DeliveryStatus::Completed);
    EXPECT_TRUE(driverPtr->isAvailable());
    EXPECT_EQ(vehicle->status(), VehicleStatus::Available);
    EXPECT_EQ(orderValue->status(), OrderStatus::Delivered);
    EXPECT_NEAR(delivery.durationHours(), 24.0, 1e-9);
    EXPECT_TRUE(delivery.isOnTime());
    EXPECT_THROW(delivery.complete(date(2, 1, 2026)), InvalidOrderStateException);
}

TEST(DeliveryTest, FailsInProgressDelivery) {
    const auto orderValue = order();
    const auto driverPtr = std::make_shared<Driver>(driver());
    const auto vehicle = std::make_shared<Truck>(truck());
    orderValue->assignDriver(*driverPtr);
    orderValue->assignVehicle(*vehicle);

    Delivery delivery(2, orderValue, driverPtr, vehicle);
    EXPECT_THROW(delivery.fail(), InvalidOrderStateException);
    delivery.start(date(1, 1, 2026));
    delivery.fail();
    EXPECT_EQ(delivery.status(), DeliveryStatus::Failed);
    EXPECT_EQ(orderValue->status(), OrderStatus::Cancelled);
    EXPECT_TRUE(driverPtr->isAvailable());
    EXPECT_EQ(vehicle->status(), VehicleStatus::Available);
    EXPECT_THROW(delivery.fail(), InvalidOrderStateException);
}

TEST(DeliveryTest, RejectsNotReadyOrderAndInvalidCompletionDate) {
    const auto orderValue = order();
    const auto driverPtr = std::make_shared<Driver>(driver());
    const auto vehicle = std::make_shared<Truck>(truck());
    Delivery delivery(4, orderValue, driverPtr, vehicle);
    EXPECT_THROW(delivery.start(date(1, 1, 2026)), InvalidOrderStateException);

    orderValue->assignDriver(*driverPtr);
    orderValue->assignVehicle(*vehicle);
    delivery.start(date(5, 1, 2026));
    EXPECT_THROW(delivery.complete(date(1, 1, 2026)), InvalidOrderStateException);
}

TEST(DeliveryTest, ReportsLateCompletionAndRejectsInvalidData) {
    const auto orderValue = order();
    const auto driverPtr = std::make_shared<Driver>(driver());
    const auto vehicle = std::make_shared<Truck>(truck());
    orderValue->assignDriver(*driverPtr);
    orderValue->assignVehicle(*vehicle);

    Delivery delivery(3, orderValue, driverPtr, vehicle);
    delivery.start(date(1, 1, 2026));
    delivery.complete(date(11, 1, 2026));
    EXPECT_FALSE(delivery.isOnTime());

    EXPECT_THROW(Delivery(0, orderValue, driverPtr, vehicle), TransportException);
    EXPECT_THROW(Delivery(1, nullptr, driverPtr, vehicle), TransportException);
}

TEST(TrackingSystemTest, StoresAndReturnsEvents) {
    TrackingSystem tracking;
    EXPECT_FALSE(tracking.hasHistory(1));
    tracking.addEvent(1, "Created", date(1, 1, 2026));
    tracking.addEvent(1, "Assigned", date(2, 1, 2026));

    EXPECT_TRUE(tracking.hasHistory(1));
    EXPECT_EQ(tracking.currentStatus(1), "Assigned");
    EXPECT_EQ(tracking.lastEvent(1), "02.01.2026 Assigned");
    ASSERT_EQ(tracking.historyFor(1).size(), 2);
    EXPECT_EQ(tracking.historyFor(1)[0], "01.01.2026 Created");

    tracking.clear();
    EXPECT_FALSE(tracking.hasHistory(1));
    EXPECT_THROW(tracking.historyFor(1), OrderNotFoundException);
    EXPECT_THROW(tracking.currentStatus(1), OrderNotFoundException);
    EXPECT_THROW(tracking.lastEvent(1), OrderNotFoundException);
    EXPECT_THROW(tracking.addEvent(0, "Bad", date()), TransportException);
    EXPECT_THROW(tracking.addEvent(1, "", date()), TransportException);
}

TEST(LogisticsManagerTest, AssignsAndCompletesDelivery) {
    LogisticsManager manager;
    auto dispatcher = std::make_shared<Dispatcher>(
        1, "Name", address(), "phone", salary(), date(), "Moscow", 2, "Day");
    manager.addDispatcher(dispatcher);

    const auto orderValue = order();
    const auto driverPtr = std::make_shared<Driver>(driver());
    const auto vehicle = std::make_shared<Truck>(truck());
    orderValue->assignDriver(*driverPtr);
    orderValue->assignVehicle(*vehicle);

    Delivery& delivery = manager.assignDelivery(1, orderValue, driverPtr, vehicle);
    EXPECT_EQ(dispatcher->activeOrders(), 1);
    EXPECT_EQ(&manager.findDelivery(1), &delivery);
    delivery.start(date(1, 1, 2026));
    manager.completeDelivery(1, date(2, 1, 2026));
    EXPECT_EQ(dispatcher->activeOrders(), 0);
    EXPECT_NE(manager.dailyReport().find("completed=1"), std::string::npos);
}

TEST(LogisticsManagerTest, ReportsAllDeliveryStates) {
    LogisticsManager manager;
    manager.addDispatcher(std::make_shared<Dispatcher>(
        1, "Name", address(), "phone", salary(), date(), "Moscow", 2, "Day"));

    const auto firstOrder = order(1);
    const auto firstDriver = std::make_shared<Driver>(driver(1));
    const auto firstVehicle = std::make_shared<Truck>(truck("A1"));
    firstOrder->assignDriver(*firstDriver);
    firstOrder->assignVehicle(*firstVehicle);
    manager.assignDelivery(1, firstOrder, firstDriver, firstVehicle);

    const auto secondOrder = order(2);
    const auto secondDriver = std::make_shared<Driver>(driver(2));
    const auto secondVehicle = std::make_shared<Truck>(truck("A2"));
    secondOrder->assignDriver(*secondDriver);
    secondOrder->assignVehicle(*secondVehicle);
    manager.assignDelivery(2, secondOrder, secondDriver, secondVehicle);

    Delivery& secondDelivery = manager.findDelivery(2);
    secondDelivery.start(date(1, 1, 2026));
    EXPECT_NE(manager.dailyReport().find("planned=1"), std::string::npos);
    EXPECT_NE(manager.dailyReport().find("in progress=1"), std::string::npos);
    secondDelivery.fail();

    Delivery& firstDelivery = manager.findDelivery(1);
    firstDelivery.start(date(1, 1, 2026));
    manager.completeDelivery(1, date(2, 1, 2026));

    const std::string report = manager.dailyReport();
    EXPECT_NE(report.find("completed=1"), std::string::npos);
    EXPECT_NE(report.find("failed=1"), std::string::npos);
}

TEST(LogisticsManagerTest, RejectsMissingResources) {
    LogisticsManager manager;
    EXPECT_THROW(manager.addDispatcher(nullptr), TransportException);
    EXPECT_THROW((void)manager.findDelivery(99), OrderNotFoundException);

    const auto orderValue = order();
    const auto driverPtr = std::make_shared<Driver>(driver());
    const auto vehicle = std::make_shared<Truck>(truck());
    orderValue->assignDriver(*driverPtr);
    orderValue->assignVehicle(*vehicle);
    EXPECT_THROW((void)manager.assignDelivery(1, orderValue, driverPtr, vehicle),
                 TransportException);
}

} // namespace
