#include "TestHelpers.h"

#include <gtest/gtest.h>

#include <cmath>
#include <limits>

#include "transport/Enums.h"
#include "transport/exceptions/InvalidDateException.h"
#include "transport/exceptions/InvalidLocationException.h"
#include "transport/exceptions/InvalidMoneyException.h"

namespace {

using namespace transport;
using namespace transport::test;

TEST(AddressTest, FormatsAndComparesCities) {
    const Address first = address("Main", "Moscow", "101000", "1");
    const Address second = address("Second", "Moscow", "101001", "2");
    const Address third = address("Main", "Kazan", "420000", "1");

    EXPECT_EQ(first.format(), "Moscow, Main, 1, 101000");
    EXPECT_TRUE(first.sameCity(second));
    EXPECT_FALSE(first.sameCity(third));
}

TEST(AddressTest, RejectsEmptyFields) {
    EXPECT_THROW(Address("", "Moscow", "101000", "1"), InvalidLocationException);
    EXPECT_THROW(Address("Main", "", "101000", "1"), InvalidLocationException);
    EXPECT_THROW(Address("Main", "Moscow", "", "1"), InvalidLocationException);
    EXPECT_THROW(Address("Main", "Moscow", "101000", ""), InvalidLocationException);
}

TEST(GPSPositionTest, ValidatesAndComputesDistance) {
    const GPSPosition moscow(55.7558, 37.6173);
    const GPSPosition spb(59.9343, 30.3351);

    EXPECT_TRUE(moscow.isValid());
    EXPECT_NEAR(moscow.distanceTo(spb), 634.0, 10.0);
    EXPECT_TRUE(moscow.approxEquals(GPSPosition(55.7558, 37.6173)));
    EXPECT_FALSE(moscow.approxEquals(spb));
}

TEST(GPSPositionTest, RejectsInvalidCoordinates) {
    EXPECT_THROW(GPSPosition(91.0, 0.0), InvalidLocationException);
    EXPECT_THROW(GPSPosition(-91.0, 0.0), InvalidLocationException);
    EXPECT_THROW(GPSPosition(0.0, 181.0), InvalidLocationException);
    EXPECT_THROW(GPSPosition(0.0, -181.0), InvalidLocationException);
    EXPECT_THROW(GPSPosition(std::numeric_limits<double>::quiet_NaN(), 0.0),
                 InvalidLocationException);
}

TEST(MoneyTest, ConstructsAndFormats) {
    const Money money(123.456, "RUB");
    EXPECT_NEAR(money.amount(), 123.46, 1e-9);
    EXPECT_EQ(money.currency(), "RUB");
    EXPECT_EQ(money.format(), "123.46 RUB");
    EXPECT_TRUE(money == Money(123.46, "RUB"));
    EXPECT_FALSE(money == Money(123.46, "USD"));
}

TEST(MoneyTest, RejectsInvalidAmountsAndCurrencies) {
    EXPECT_THROW(Money(std::numeric_limits<double>::quiet_NaN(), "RUB"), InvalidMoneyException);
    EXPECT_THROW(Money(1.0, "RU"), InvalidMoneyException);
    EXPECT_THROW(Money(1.0, "RUBLES"), InvalidMoneyException);
}

TEST(MoneyTest, ArithmeticAndPredicates) {
    const Money first(100.0, "RUB");
    const Money second(40.0, "RUB");

    EXPECT_EQ(first.add(second).amount(), 140.0);
    EXPECT_EQ(first.subtract(second).amount(), 60.0);
    EXPECT_EQ(first.multiply(1.5).amount(), 150.0);
    EXPECT_FALSE(first.isNegative());
    EXPECT_FALSE(first.isZero());
    EXPECT_TRUE(Money(0.0, "RUB").isZero());
    EXPECT_TRUE(first.subtract(Money(200.0, "RUB")).isNegative());
}

TEST(MoneyTest, RejectsCurrencyMismatchAndBadMultiplier) {
    const Money rub(100.0, "RUB");
    const Money usd(100.0, "USD");
    EXPECT_THROW(rub.add(usd), InvalidMoneyException);
    EXPECT_THROW(rub.subtract(usd), InvalidMoneyException);
    EXPECT_THROW(rub.multiply(std::numeric_limits<double>::infinity()), InvalidMoneyException);
}

TEST(TransportDateTest, ValidatesCalendarDates) {
    EXPECT_TRUE(TransportDate(29, 2, 2024).isValid());
    EXPECT_THROW(TransportDate(29, 2, 2023), InvalidDateException);
    EXPECT_THROW(TransportDate(31, 4, 2026), InvalidDateException);
    EXPECT_THROW(TransportDate(0, 1, 2026), InvalidDateException);
    EXPECT_THROW(TransportDate(1, 13, 2026), InvalidDateException);
    EXPECT_THROW(TransportDate(1, 1, 0), InvalidDateException);
    EXPECT_THROW(TransportDate(31, 2, 2026), InvalidDateException);
}

TEST(TransportDateTest, ComparesAndAddsDays) {
    const TransportDate first(31, 12, 2025);
    const TransportDate second(1, 1, 2026);

    EXPECT_TRUE(first.isBefore(second));
    EXPECT_TRUE(second.isAfter(first));
    EXPECT_TRUE(first.sameDay(TransportDate(31, 12, 2025)));
    EXPECT_EQ(first.addDays(1).toString(), "01.01.2026");
    EXPECT_EQ(first.daysUntil(second), 1);
    EXPECT_EQ(second.daysUntil(first), -1);
    EXPECT_EQ(first.toString(), "31.12.2025");
}

TEST(EnumTest, ConvertsEveryValueAndUnknown) {
    EXPECT_STREQ(toString(VehicleStatus::Available), "Available");
    EXPECT_STREQ(toString(VehicleStatus::OnRoute), "OnRoute");
    EXPECT_STREQ(toString(VehicleStatus::Maintenance), "Maintenance");
    EXPECT_STREQ(toString(VehicleStatus::Inactive), "Inactive");
    EXPECT_STREQ(toString(static_cast<VehicleStatus>(99)), "Unknown");

    EXPECT_STREQ(toString(OrderStatus::Created), "Created");
    EXPECT_STREQ(toString(OrderStatus::Assigned), "Assigned");
    EXPECT_STREQ(toString(OrderStatus::InTransit), "InTransit");
    EXPECT_STREQ(toString(OrderStatus::Delivered), "Delivered");
    EXPECT_STREQ(toString(OrderStatus::Cancelled), "Cancelled");
    EXPECT_STREQ(toString(static_cast<OrderStatus>(99)), "Unknown");

    EXPECT_STREQ(toString(DeliveryStatus::Planned), "Planned");
    EXPECT_STREQ(toString(DeliveryStatus::InProgress), "InProgress");
    EXPECT_STREQ(toString(DeliveryStatus::Completed), "Completed");
    EXPECT_STREQ(toString(DeliveryStatus::Failed), "Failed");
    EXPECT_STREQ(toString(static_cast<DeliveryStatus>(99)), "Unknown");

    EXPECT_STREQ(toString(PaymentStatus::Pending), "Pending");
    EXPECT_STREQ(toString(PaymentStatus::Completed), "Completed");
    EXPECT_STREQ(toString(PaymentStatus::Refunded), "Refunded");
    EXPECT_STREQ(toString(PaymentStatus::Failed), "Failed");
    EXPECT_STREQ(toString(static_cast<PaymentStatus>(99)), "Unknown");
}

} // namespace
