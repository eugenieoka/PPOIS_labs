#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "transport/analytics/ServiceQualityMonitor.h"

namespace transport {

class Client;
class CustomerFeedback;

class CustomerService {
public:
    void registerClient(std::shared_ptr<Client> client);
    [[nodiscard]] CustomerFeedback fileComplaint(int64_t clientId, int64_t orderId,
                                                 int rating, const std::string& comment,
                                                 const TransportDate& date);
    [[nodiscard]] std::shared_ptr<Client> findClient(int64_t clientId) const;
    [[nodiscard]] const std::vector<std::shared_ptr<Client>>& clients() const noexcept {
        return clients_;
    }
    [[nodiscard]] const ServiceQualityMonitor& monitor() const noexcept { return monitor_; }

private:
    std::vector<std::shared_ptr<Client>> clients_;
    ServiceQualityMonitor monitor_;
};

} // namespace transport
