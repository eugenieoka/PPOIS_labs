#include "TestHelpers.h"

#include <gtest/gtest.h>

#include "transport/cargo/Cargo.h"
#include "transport/cargo/FragileCargo.h"
#include "transport/cargo/PerishableCargo.h"
#include "transport/exceptions/InvalidCargoDataException.h"
#include "transport/exceptions/TransportException.h"
#include "transport/exceptions/WarehouseCapacityException.h"
#include "transport/warehouse/Rack.h"
#include "transport/warehouse/StorageUnit.h"
#include "transport/warehouse/StorageZone.h"
#include "transport/warehouse/Warehouse.h"

namespace {

using namespace transport;
using namespace transport::test;

TEST(CargoTest, ComputesDensityAndCost) {
    const Cargo cargoValue(1, "Boxes", 100.0, 2.0, Money(1000.0, "RUB"));
    EXPECT_NEAR(cargoValue.density(), 50.0, 1e-9);
    EXPECT_FALSE(cargoValue.isOverweight(100.0));
    EXPECT_TRUE(cargoValue.isOverweight(99.0));
    EXPECT_NEAR(cargoValue.handlingCost().amount(), 1000.0, 1e-9);
    EXPECT_EQ(cargoValue.typeName(), "Cargo");
    EXPECT_NE(cargoValue.summary().find("Boxes"), std::string::npos);
    EXPECT_THROW((void)cargoValue.isOverweight(0.0), InvalidCargoDataException);
}

TEST(CargoTest, RejectsInvalidData) {
    EXPECT_THROW(Cargo(0, "Boxes", 1.0, 1.0, Money(1.0, "RUB")),
                 InvalidCargoDataException);
    EXPECT_THROW(Cargo(1, "", 1.0, 1.0, Money(1.0, "RUB")),
                 InvalidCargoDataException);
    EXPECT_THROW(Cargo(1, "Boxes", 0.0, 1.0, Money(1.0, "RUB")),
                 InvalidCargoDataException);
    EXPECT_THROW(Cargo(1, "Boxes", 1.0, 0.0, Money(1.0, "RUB")),
                 InvalidCargoDataException);
    EXPECT_THROW(Cargo(1, "Boxes", 1.0, 1.0, Money(-1.0, "RUB")),
                 InvalidCargoDataException);
}

TEST(FragileCargoTest, AddsSurchargeAndRequiresPackaging) {
    const FragileCargo fragile(2, "Glass", 50.0, 1.0, Money(2000.0, "RUB"), "Crate", 30);
    EXPECT_TRUE(fragile.requiresSpecialPackaging());
    EXPECT_EQ(fragile.packaging(), "Crate");
    EXPECT_EQ(fragile.maxDropHeightCm(), 30);
    EXPECT_EQ(fragile.typeName(), "FragileCargo");
    EXPECT_NEAR(fragile.handlingCost().amount(), 1000.0, 1e-9);
}

TEST(FragileCargoTest, RejectsInvalidPackaging) {
    EXPECT_THROW(FragileCargo(2, "Glass", 1.0, 1.0, Money(1.0, "RUB"), "", 30),
                 InvalidCargoDataException);
    EXPECT_THROW(FragileCargo(2, "Glass", 1.0, 1.0, Money(1.0, "RUB"), "Crate", 0),
                 InvalidCargoDataException);
}

TEST(PerishableCargoTest, TracksExpiryAndRefrigeration) {
    const PerishableCargo perishable(3, "Milk", 40.0, 0.5, Money(500.0, "RUB"),
                                     date(10, 1, 2026), 2.0, 6.0);
    EXPECT_FALSE(perishable.isExpiredOn(date(10, 1, 2026)));
    EXPECT_TRUE(perishable.isExpiredOn(date(11, 1, 2026)));
    EXPECT_TRUE(perishable.requiresRefrigeration());
    EXPECT_EQ(perishable.remainingDays(date(5, 1, 2026)), 5);
    EXPECT_EQ(perishable.remainingDays(date(20, 1, 2026)), 0);
    EXPECT_EQ(perishable.typeName(), "PerishableCargo");
    EXPECT_NEAR(perishable.handlingCost().amount(), 700.0, 1e-9);

    const PerishableCargo warm(4, "Bread", 10.0, 0.1, Money(100.0, "RUB"),
                               date(10, 1, 2026), 10.0, 20.0);
    EXPECT_FALSE(warm.requiresRefrigeration());
}

TEST(PerishableCargoTest, RejectsInvalidTemperatureRange) {
    EXPECT_THROW(PerishableCargo(3, "Milk", 1.0, 1.0, Money(1.0, "RUB"),
                                 date(), 6.0, 2.0), InvalidCargoDataException);
}

TEST(RackTest, PlacesAndTakesCargo) {
    Rack rack(1, 3, 1000.0);
    EXPECT_NEAR(rack.freeCapacity(), 1000.0, 1e-9);
    EXPECT_TRUE(rack.isEmpty());

    const auto cargoValue = cargo(1, 100.0);
    rack.place(*cargoValue);
    EXPECT_NEAR(rack.freeCapacity(), 900.0, 1e-9);
    EXPECT_FALSE(rack.isEmpty());
    EXPECT_THROW(rack.place(*cargoValue), TransportException);

    rack.take(*cargoValue);
    EXPECT_NEAR(rack.freeCapacity(), 1000.0, 1e-9);
    EXPECT_TRUE(rack.isEmpty());
    EXPECT_THROW(rack.take(*cargoValue), TransportException);

    EXPECT_THROW(rack.place(*cargo(2, 2000.0)), WarehouseCapacityException);
}

TEST(RackTest, RejectsInvalidConstruction) {
    EXPECT_THROW(Rack(0, 1, 1.0), TransportException);
    EXPECT_THROW(Rack(1, 0, 1.0), TransportException);
    EXPECT_THROW(Rack(1, 1, 0.0), TransportException);
}

TEST(StorageUnitTest, StoresAndReleasesCargo) {
    StorageUnit unit("U-1", 1000.0, "Zone A");
    const auto cargoValue = cargo(1, 100.0);
    EXPECT_TRUE(unit.canStore(*cargoValue));
    EXPECT_FALSE(unit.canStore(*cargo(2, 2000.0)));

    unit.store(*cargoValue);
    EXPECT_NEAR(unit.occupancy(), 100.0, 1e-9);
    EXPECT_THROW(unit.store(*cargoValue), TransportException);

    unit.release(*cargoValue);
    EXPECT_NEAR(unit.occupancy(), 0.0, 1e-9);
    EXPECT_THROW(unit.release(*cargoValue), TransportException);
    EXPECT_THROW(unit.store(*cargo(3, 2000.0)), WarehouseCapacityException);
}

TEST(StorageUnitTest, RejectsInvalidConstruction) {
    EXPECT_THROW(StorageUnit("", 1.0, "Zone"), TransportException);
    EXPECT_THROW(StorageUnit("U", 1.0, ""), TransportException);
    EXPECT_THROW(StorageUnit("U", 0.0, "Zone"), TransportException);
}

TEST(StorageZoneTest, FindsFreeRacksAndChecksTemperature) {
    StorageZone zone("Cold", 4.0);
    zone.addRack(Rack(1, 2, 100.0));
    zone.addRack(Rack(2, 2, 1000.0));

    EXPECT_NEAR(zone.totalCapacity(), 1100.0, 1e-9);
    EXPECT_NEAR(zone.occupancy(), 0.0, 1e-9);
    Rack* rack = zone.findFreeRack(*cargo(1, 500.0));
    ASSERT_NE(rack, nullptr);
    EXPECT_EQ(rack->number(), 2);
    EXPECT_EQ(zone.findFreeRack(*cargo(2, 2000.0)), nullptr);

    const auto perishable = perishableCargo();
    EXPECT_TRUE(zone.accepts(*perishable));
    StorageZone warm("Warm", 20.0);
    EXPECT_FALSE(warm.accepts(*perishable));
}

TEST(StorageZoneTest, RejectsEmptyName) {
    EXPECT_THROW(StorageZone("", 0.0), TransportException);
}

TEST(WarehouseTest, ReceivesShipsAndReportsInventory) {
    Warehouse warehouseValue("Central", address());
    StorageZone zone("Main", 20.0);
    zone.addRack(Rack(1, 3, 1000.0));
    warehouseValue.addZone(zone);

    StorageUnit unit("U-1", 1000.0, "Main");
    const auto cargoValue = cargo(1, 100.0);

    EXPECT_NEAR(warehouseValue.totalCapacity(), 1000.0, 1e-9);
    EXPECT_NEAR(warehouseValue.occupancy(), 0.0, 1e-9);

    warehouseValue.receiveCargo(*cargoValue, unit);
    EXPECT_NEAR(warehouseValue.inventoryValue().amount(), 1000.0, 1e-9);
    warehouseValue.shipCargo(*cargoValue, unit);
    EXPECT_NEAR(warehouseValue.inventoryValue().amount(), 0.0, 1e-9);
    EXPECT_EQ(warehouseValue.name(), "Central");
}

TEST(WarehouseTest, RejectsEmptyName) {
    EXPECT_THROW(Warehouse("", address()), TransportException);
}

} // namespace
