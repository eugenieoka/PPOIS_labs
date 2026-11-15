#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "transport/orders/Order.h"

namespace transport {

class Cargo;
class Client;

class ExpressDelivery : public Order {
public:
    ExpressDelivery(int64_t orderId, std::shared_ptr<Client> client,
                    std::shared_ptr<Cargo> cargo, Route route,
                    TransportDate createdOn, TransportDate deadline,
                    int guaranteedHours, double surchargeRate);

    [[nodiscard]] int guaranteedHours() const noexcept { return guaranteedHours_; }
    [[nodiscard]] double surchargeRate() const noexcept { return surchargeRate_; }

    [[nodiscard]] Money calculatePrice() override;
    [[nodiscard]] bool isExpress() const noexcept;
    [[nodiscard]] TransportDate estimatedDeliveryDate() const;

private:
    int guaranteedHours_ = 0;
    double surchargeRate_ = 0.0;
};

} // namespace transport
