#include "transport/orders/LogisticsManager.h"

#include "transport/exceptions/OrderNotFoundException.h"
#include "transport/exceptions/TransportException.h"
#include "transport/orders/Order.h"
#include "transport/staff/Dispatcher.h"

#include <sstream>
#include <utility>

namespace transport {

void LogisticsManager::addDispatcher(std::shared_ptr<Dispatcher> dispatcher) {
    if (!dispatcher) {
        throw TransportException("Dispatcher must not be null");
    }
    dispatchers_.push_back(std::move(dispatcher));
}

Delivery& LogisticsManager::assignDelivery(int64_t deliveryId, std::shared_ptr<Order> order,
                                           std::shared_ptr<Driver> driver,
                                           std::shared_ptr<Vehicle> vehicle) {
    std::shared_ptr<Dispatcher> selected;
    for (const auto& dispatcher : dispatchers_) {
        if (dispatcher->canHandleMore()) {
            selected = dispatcher;
            break;
        }
    }
    if (!selected) {
        throw TransportException("No dispatcher is available");
    }
    selected->assignOrder();
    deliveries_.emplace_back(deliveryId, std::move(order), std::move(driver), std::move(vehicle));
    return deliveries_.back();
}

void LogisticsManager::completeDelivery(int64_t deliveryId, const TransportDate& date) {
    Delivery& delivery = findDelivery(deliveryId);
    delivery.complete(date);
    for (const auto& dispatcher : dispatchers_) {
        if (dispatcher->activeOrders() > 0) {
            dispatcher->releaseOrder();
            break;
        }
    }
}

Delivery& LogisticsManager::findDelivery(int64_t deliveryId) {
    for (Delivery& delivery : deliveries_) {
        if (delivery.deliveryId() == deliveryId) {
            return delivery;
        }
    }
    throw OrderNotFoundException("Delivery not found");
}

std::string LogisticsManager::dailyReport() const {
    int planned = 0;
    int inProgress = 0;
    int completed = 0;
    int failed = 0;
    for (const Delivery& delivery : deliveries_) {
        switch (delivery.status()) {
            case DeliveryStatus::Planned:    ++planned; break;
            case DeliveryStatus::InProgress: ++inProgress; break;
            case DeliveryStatus::Completed:  ++completed; break;
            case DeliveryStatus::Failed:     ++failed; break;
        }
    }
    std::ostringstream out;
    out << "planned=" << planned << ", in progress=" << inProgress
        << ", completed=" << completed << ", failed=" << failed;
    return out.str();
}

} // namespace transport
