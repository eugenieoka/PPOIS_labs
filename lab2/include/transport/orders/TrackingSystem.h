#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "transport/core/TransportDate.h"

namespace transport {

class TrackingSystem {
public:
    void addEvent(int64_t orderId, const std::string& status, const TransportDate& date);
    [[nodiscard]] std::vector<std::string> historyFor(int64_t orderId) const;
    [[nodiscard]] std::string currentStatus(int64_t orderId) const;
    [[nodiscard]] std::string lastEvent(int64_t orderId) const;
    [[nodiscard]] bool hasHistory(int64_t orderId) const noexcept;
    void clear() noexcept;

private:
    std::map<int64_t, std::vector<std::pair<TransportDate, std::string>>> events_;
};

} // namespace transport
