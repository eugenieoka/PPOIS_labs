#include "transport/orders/ExpressDelivery.h"

#include "transport/Constants.h"
#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {

ExpressDelivery::ExpressDelivery(int64_t orderId, std::shared_ptr<Client> client,
                                 std::shared_ptr<Cargo> cargo, Route route,
                                 TransportDate createdOn, TransportDate deadline,
                                 int guaranteedHours, double surchargeRate)
    : Order(orderId, std::move(client), std::move(cargo), std::move(route), createdOn, deadline),
      guaranteedHours_(guaranteedHours),
      surchargeRate_(surchargeRate) {
    if (guaranteedHours_ <= 0) {
        throw TransportException("Express delivery hours must be positive");
    }
    if (surchargeRate_ < 0.0) {
        throw TransportException("Express delivery surcharge must not be negative");
    }
}

Money ExpressDelivery::calculatePrice() {
    const Money base = Order::calculatePrice();
    const Money withSurcharge = base.multiply(1.0 + surchargeRate_);
    setPrice(withSurcharge);
    return withSurcharge;
}

bool ExpressDelivery::isExpress() const noexcept {
    return true;
}

TransportDate ExpressDelivery::estimatedDeliveryDate() const {
    const int days = (guaranteedHours_ + constants::kHoursPerDay - 1) / constants::kHoursPerDay;
    return createdOn().addDays(days);
}

} // namespace transport
