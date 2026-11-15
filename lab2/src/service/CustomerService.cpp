#include "transport/service/CustomerService.h"

#include "transport/analytics/CustomerFeedback.h"
#include "transport/exceptions/OrderNotFoundException.h"
#include "transport/exceptions/TransportException.h"
#include "transport/orders/Client.h"

#include <utility>

namespace transport {

void CustomerService::registerClient(std::shared_ptr<Client> client) {
    if (!client) {
        throw TransportException("Client must not be null");
    }
    clients_.push_back(std::move(client));
}

CustomerFeedback CustomerService::fileComplaint(int64_t clientId, int64_t orderId,
                                                int rating, const std::string& comment,
                                                const TransportDate& date) {
    const std::shared_ptr<Client> client = findClient(clientId);
    (void)client;
    CustomerFeedback feedback(clientId, orderId, rating, comment, date);
    monitor_.addFeedback(feedback);
    return feedback;
}

std::shared_ptr<Client> CustomerService::findClient(int64_t clientId) const {
    for (const auto& client : clients_) {
        if (client->clientId() == clientId) {
            return client;
        }
    }
    throw OrderNotFoundException("Client not found");
}

} // namespace transport
