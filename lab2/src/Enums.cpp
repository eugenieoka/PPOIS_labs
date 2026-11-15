#include "transport/Enums.h"

namespace transport {

const char* toString(VehicleStatus status) noexcept {
    switch (status) {
        case VehicleStatus::Available:   return "Available";
        case VehicleStatus::OnRoute:     return "OnRoute";
        case VehicleStatus::Maintenance: return "Maintenance";
        case VehicleStatus::Inactive:    return "Inactive";
    }
    return "Unknown";
}

const char* toString(OrderStatus status) noexcept {
    switch (status) {
        case OrderStatus::Created:   return "Created";
        case OrderStatus::Assigned:  return "Assigned";
        case OrderStatus::InTransit: return "InTransit";
        case OrderStatus::Delivered: return "Delivered";
        case OrderStatus::Cancelled: return "Cancelled";
    }
    return "Unknown";
}

const char* toString(DeliveryStatus status) noexcept {
    switch (status) {
        case DeliveryStatus::Planned:    return "Planned";
        case DeliveryStatus::InProgress: return "InProgress";
        case DeliveryStatus::Completed:  return "Completed";
        case DeliveryStatus::Failed:     return "Failed";
    }
    return "Unknown";
}

const char* toString(PaymentStatus status) noexcept {
    switch (status) {
        case PaymentStatus::Pending:   return "Pending";
        case PaymentStatus::Completed: return "Completed";
        case PaymentStatus::Refunded:  return "Refunded";
        case PaymentStatus::Failed:    return "Failed";
    }
    return "Unknown";
}

} // namespace transport
