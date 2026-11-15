#include "transport/orders/TrackingSystem.h"

#include "transport/exceptions/OrderNotFoundException.h"
#include "transport/exceptions/TransportException.h"

namespace transport {

void TrackingSystem::addEvent(int64_t orderId, const std::string& status,
                              const TransportDate& date) {
    if (orderId <= 0) {
        throw TransportException("Tracking order id must be positive");
    }
    if (status.empty()) {
        throw TransportException("Tracking status must not be empty");
    }
    events_[orderId].emplace_back(date, status);
}

std::vector<std::string> TrackingSystem::historyFor(int64_t orderId) const {
    const auto found = events_.find(orderId);
    if (found == events_.end()) {
        throw OrderNotFoundException("No tracking history for order");
    }
    std::vector<std::string> history;
    history.reserve(found->second.size());
    for (const auto& event : found->second) {
        history.push_back(event.first.toString() + " " + event.second);
    }
    return history;
}

std::string TrackingSystem::currentStatus(int64_t orderId) const {
    const auto found = events_.find(orderId);
    if (found == events_.end() || found->second.empty()) {
        throw OrderNotFoundException("No tracking events for order");
    }
    return found->second.back().second;
}

std::string TrackingSystem::lastEvent(int64_t orderId) const {
    const auto found = events_.find(orderId);
    if (found == events_.end() || found->second.empty()) {
        throw OrderNotFoundException("No tracking events for order");
    }
    const auto& event = found->second.back();
    return event.first.toString() + " " + event.second;
}

bool TrackingSystem::hasHistory(int64_t orderId) const noexcept {
    const auto found = events_.find(orderId);
    return found != events_.end() && !found->second.empty();
}

void TrackingSystem::clear() noexcept {
    events_.clear();
}

} // namespace transport
