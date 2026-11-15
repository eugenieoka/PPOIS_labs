#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "transport/analytics/ServiceQualityMonitor.h"
#include "transport/service/CustomerService.h"
#include "transport/service/LoyaltyProgram.h"

namespace transport {

class Client;
class CustomerFeedback;

class ServiceManagement {
public:
    ServiceManagement() = default;
    ServiceManagement(LoyaltyProgram loyalty, CustomerService customerService);

    void onboardClient(std::shared_ptr<Client> client);
    [[nodiscard]] CustomerFeedback processComplaint(int64_t clientId, int64_t orderId,
                                                    int rating, const std::string& comment,
                                                    const TransportDate& date);
    [[nodiscard]] std::string loyaltyReport(const Client& client) const;
    [[nodiscard]] const ServiceQualityMonitor& monitor() const noexcept { return monitor_; }

private:
    LoyaltyProgram loyalty_;
    CustomerService customerService_;
    ServiceQualityMonitor monitor_;
};

} // namespace transport
