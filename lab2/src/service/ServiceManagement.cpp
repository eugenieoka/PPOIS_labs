#include "transport/service/ServiceManagement.h"

#include "transport/analytics/CustomerFeedback.h"
#include "transport/orders/Client.h"

#include <sstream>
#include <utility>

namespace transport {

ServiceManagement::ServiceManagement(LoyaltyProgram loyalty, CustomerService customerService)
    : loyalty_(std::move(loyalty)), customerService_(std::move(customerService)) {}

void ServiceManagement::onboardClient(std::shared_ptr<Client> client) {
    customerService_.registerClient(std::move(client));
}

CustomerFeedback ServiceManagement::processComplaint(int64_t clientId, int64_t orderId,
                                                     int rating, const std::string& comment,
                                                     const TransportDate& date) {
    CustomerFeedback feedback = customerService_.fileComplaint(clientId, orderId,
                                                               rating, comment, date);
    monitor_.addFeedback(feedback);
    return feedback;
}

std::string ServiceManagement::loyaltyReport(const Client& client) const {
    std::ostringstream out;
    out << client.fullName() << ": " << client.loyaltyPoints() << " points, tier "
        << loyalty_.tierFor(client.loyaltyPoints());
    return out.str();
}

} // namespace transport
