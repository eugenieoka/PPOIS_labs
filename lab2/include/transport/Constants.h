#pragma once

namespace transport::constants {

inline constexpr double kEarthRadiusKm = 6371.0;
inline constexpr double kCentsPerCurrencyUnit = 100.0;
inline constexpr double kHandlingRatePerKg = 10.0;
inline constexpr double kFragileHandlingSurcharge = 500.0;
inline constexpr double kPerishableHandlingSurcharge = 300.0;
inline constexpr double kPricePerKm = 25.0;
inline constexpr double kMaintenanceMileageLimitKm = 10000.0;
inline constexpr double kDefaultVatRate = 0.20;
inline constexpr double kDefaultIncomeTaxRate = 0.13;
inline constexpr double kRefrigerationThresholdC = 8.0;
inline constexpr double kVehicleLoadingCost = 500.0;
inline constexpr double kThreeAxleCapacityMultiplier = 1.2;
inline constexpr double kMaxDriverRating = 5.0;
inline constexpr double kMinimumDriverRating = 0.0;
inline constexpr int kMinimumTruckAxles = 2;
inline constexpr int kMinimumRackLevels = 1;
inline constexpr int kMinimumLoyaltyPoints = 0;
inline constexpr int kSilverLoyaltyThreshold = 100;
inline constexpr int kGoldLoyaltyThreshold = 500;
inline constexpr double kSilverDiscountRate = 0.05;
inline constexpr double kGoldDiscountRate = 0.10;
inline constexpr int kComplaintRatingThreshold = 2;
inline constexpr double kEpsilon = 1e-9;
inline constexpr int kDaysPerYear = 365;
inline constexpr int kHoursPerDay = 24;

} // namespace transport::constants
