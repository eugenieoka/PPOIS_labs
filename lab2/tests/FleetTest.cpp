#include "TestHelpers.h"

#include <gtest/gtest.h>

#include "transport/exceptions/TransportException.h"
#include "transport/fleet/Insurance.h"
#include "transport/fleet/Machine.h"
#include "transport/fleet/MaintenanceRecord.h"
#include "transport/fleet/Trailer.h"
#include "transport/fleet/Truck.h"
#include "transport/fleet/Van.h"
#include "transport/fleet/Vehicle.h"

namespace {

using namespace transport;
using namespace transport::test;

TEST(MachineTest, StartsStopsAndConsumesFuel) {
    Machine machineValue("ENG-1", "Diesel", 12.0);
    EXPECT_FALSE(machineValue.isRunning());
    EXPECT_NEAR(machineValue.consumeFuel(2.0), 24.0, 1e-9);

    machineValue.start();
    EXPECT_TRUE(machineValue.isRunning());
    EXPECT_THROW(machineValue.start(), TransportException);
    machineValue.stop();
    EXPECT_FALSE(machineValue.isRunning());
    EXPECT_THROW(machineValue.stop(), TransportException);
    EXPECT_THROW((void)machineValue.consumeFuel(-1.0), TransportException);
}

TEST(MachineTest, RejectsInvalidConstruction) {
    EXPECT_THROW(Machine("", "Diesel", 10.0), TransportException);
    EXPECT_THROW(Machine("ENG", "", 10.0), TransportException);
    EXPECT_THROW(Machine("ENG", "Diesel", 0.0), TransportException);
}

TEST(VehicleTest, CarriesCargoAndTracksMileage) {
    Vehicle vehicleValue("A001AA", 1000.0, 20.0, 0.0, 20.0, machine());
    const auto cargoValue = cargo(1, 100.0);

    EXPECT_TRUE(vehicleValue.isAvailable());
    EXPECT_TRUE(vehicleValue.canCarry(*cargoValue));
    EXPECT_FALSE(vehicleValue.canCarry(*cargo(2, 2000.0)));
    EXPECT_NEAR(vehicleValue.baseCost(10.0).amount(), 200.0, 1e-9);
    EXPECT_EQ(vehicleValue.typeName(), "Vehicle");

    vehicleValue.addMileage(10001.0);
    EXPECT_TRUE(vehicleValue.needsMaintenance());
    EXPECT_THROW(vehicleValue.addMileage(-1.0), TransportException);
    EXPECT_THROW(vehicleValue.baseCost(-1.0), TransportException);

    vehicleValue.changeStatus(VehicleStatus::OnRoute);
    EXPECT_FALSE(vehicleValue.isAvailable());
    EXPECT_FALSE(vehicleValue.canCarry(*cargoValue));
}

TEST(VehicleTest, RejectsInvalidConstruction) {
    EXPECT_THROW(Vehicle("", 1000.0, 20.0, 0.0, 20.0, machine()), TransportException);
    EXPECT_THROW(Vehicle("A", 0.0, 20.0, 0.0, 20.0, machine()), TransportException);
    EXPECT_THROW(Vehicle("A", 1000.0, 0.0, 0.0, 20.0, machine()), TransportException);
    EXPECT_THROW(Vehicle("A", 1000.0, 20.0, -1.0, 20.0, machine()), TransportException);
    EXPECT_THROW(Vehicle("A", 1000.0, 20.0, 0.0, -1.0, machine()), TransportException);
}

TEST(TrailerTest, AttachesDetachesAndFitsCargo) {
    Trailer trailerValue(500.0, 5.0);
    EXPECT_TRUE(trailerValue.isAvailable());
    EXPECT_TRUE(trailerValue.fitsCargo(*cargo(1, 100.0)));
    EXPECT_FALSE(trailerValue.fitsCargo(*cargo(2, 600.0)));

    trailerValue.attach();
    EXPECT_FALSE(trailerValue.isAvailable());
    EXPECT_THROW(trailerValue.attach(), TransportException);
    trailerValue.detach();
    EXPECT_TRUE(trailerValue.isAvailable());
    EXPECT_THROW(trailerValue.detach(), TransportException);
    EXPECT_THROW(Trailer(0.0, 1.0), TransportException);
    EXPECT_THROW(Trailer(1.0, 0.0), TransportException);
}

TEST(TruckTest, AttachesTrailerAndComputesPayload) {
    Truck truckValue = truck();
    EXPECT_EQ(truckValue.typeName(), "Truck");
    EXPECT_NEAR(truckValue.maxPayload(), 1200.0, 1e-9);

    auto trailer = std::make_shared<Trailer>(500.0, 5.0);
    truckValue.attachTrailer(trailer);
    EXPECT_FALSE(trailer->isAvailable());
    EXPECT_EQ(truckValue.trailer(), trailer);
    EXPECT_THROW(truckValue.attachTrailer(trailer), TransportException);

    truckValue.detachTrailer();
    EXPECT_TRUE(trailer->isAvailable());
    EXPECT_THROW(truckValue.detachTrailer(), TransportException);
    EXPECT_THROW(truckValue.attachTrailer(nullptr), TransportException);
}

TEST(TruckTest, RejectsInvalidAxles) {
    EXPECT_THROW(Truck("A", 1000.0, 20.0, 0.0, 20.0, machine(), 1), TransportException);
    Truck twoAxle("A", 1000.0, 20.0, 0.0, 20.0, machine(), 2);
    EXPECT_NEAR(twoAxle.maxPayload(), 1000.0, 1e-9);
}

TEST(VanTest, ChecksRefrigerationAndLoadingCost) {
    Van refrigerated = van("V1", true);
    Van regular = van("V2", false);
    const auto perishable = perishableCargo();

    EXPECT_EQ(refrigerated.typeName(), "Van");
    EXPECT_TRUE(refrigerated.canCarryPerishable(*perishable));
    EXPECT_FALSE(regular.canCarryPerishable(*perishable));
    EXPECT_NEAR(refrigerated.loadingCost().amount(), 500.0, 1e-9);
}

TEST(VanTest, RejectsInvalidDoors) {
    EXPECT_THROW(Van("V", 500.0, 10.0, 0.0, 15.0, machine(), true, -1),
                 TransportException);
}

TEST(MaintenanceRecordTest, ComputesTaxAndRecency) {
    MaintenanceRecord record("A001AA", 1, date(1, 1, 2026), Money(1000.0, "RUB"),
                             "Oil change");
    EXPECT_NEAR(record.totalCostWithTax(0.2).amount(), 1200.0, 1e-9);
    EXPECT_TRUE(record.isRecent(date(5, 1, 2026), 10));
    EXPECT_FALSE(record.isRecent(date(20, 1, 2026), 10));
    EXPECT_FALSE(record.isRecent(date(31, 12, 2025), 10));
    EXPECT_NE(record.summary().find("Oil change"), std::string::npos);

    EXPECT_THROW(record.totalCostWithTax(-0.1), TransportException);
    EXPECT_THROW(record.totalCostWithTax(1.1), TransportException);
    EXPECT_THROW((void)record.isRecent(date(), 0), TransportException);
}

TEST(MaintenanceRecordTest, RejectsInvalidData) {
    EXPECT_THROW(MaintenanceRecord("", 1, date(), Money(1.0, "RUB"), "Desc"),
                 TransportException);
    EXPECT_THROW(MaintenanceRecord("A", 1, date(), Money(1.0, "RUB"), ""),
                 TransportException);
    EXPECT_THROW(MaintenanceRecord("A", 0, date(), Money(1.0, "RUB"), "Desc"),
                 TransportException);
    EXPECT_THROW(MaintenanceRecord("A", 1, date(), Money(-1.0, "RUB"), "Desc"),
                 TransportException);
}

TEST(InsuranceTest, ChecksActivityAndRenews) {
    Insurance insurance("POL-1", "A001AA", Money(1000.0, "RUB"),
                        date(1, 1, 2026), date(31, 12, 2026));
    EXPECT_TRUE(insurance.isActiveOn(date(1, 6, 2026)));
    EXPECT_FALSE(insurance.isActiveOn(date(1, 1, 2027)));
    EXPECT_NEAR(insurance.annualCost().amount(), 1000.0, 1e-9);

    insurance.renew(date(31, 12, 2027), Money(1200.0, "RUB"));
    EXPECT_EQ(insurance.validTo().year(), 2027);
    EXPECT_NEAR(insurance.premium().amount(), 1200.0, 1e-9);

    EXPECT_THROW(insurance.renew(date(31, 12, 2025), Money(1.0, "RUB")),
                 TransportException);
    EXPECT_THROW(insurance.renew(date(31, 12, 2028), Money(-1.0, "RUB")),
                 TransportException);
}

TEST(InsuranceTest, RejectsInvalidData) {
    EXPECT_THROW(Insurance("", "A", Money(1.0, "RUB"), date(), date()), TransportException);
    EXPECT_THROW(Insurance("P", "", Money(1.0, "RUB"), date(), date()), TransportException);
    EXPECT_THROW(Insurance("P", "A", Money(-1.0, "RUB"), date(), date()),
                 TransportException);
    EXPECT_THROW(Insurance("P", "A", Money(1.0, "RUB"), date(2, 1, 2026), date(1, 1, 2026)),
                 TransportException);
}

} // namespace
