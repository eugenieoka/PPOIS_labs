#pragma once

#include <cstdint>

namespace transport {

enum class VehicleStatus : uint8_t {
    Available = 0,
    OnRoute = 1,
    Maintenance = 2,
    Inactive = 3
};

enum class OrderStatus : uint8_t {
    Created = 0,
    Assigned = 1,
    InTransit = 2,
    Delivered = 3,
    Cancelled = 4
};

enum class DeliveryStatus : uint8_t {
    Planned = 0,
    InProgress = 1,
    Completed = 2,
    Failed = 3
};

enum class PaymentStatus : uint8_t {
    Pending = 0,
    Completed = 1,
    Refunded = 2,
    Failed = 3
};

[[nodiscard]] const char* toString(VehicleStatus status) noexcept;
[[nodiscard]] const char* toString(OrderStatus status) noexcept;
[[nodiscard]] const char* toString(DeliveryStatus status) noexcept;
[[nodiscard]] const char* toString(PaymentStatus status) noexcept;

} // namespace transport
