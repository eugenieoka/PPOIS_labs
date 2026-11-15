#include "transport/analytics/CustomerFeedback.h"

#include "transport/exceptions/InvalidRatingException.h"
#include "transport/exceptions/TransportException.h"

#include <sstream>
#include <utility>

namespace transport {
namespace {

constexpr int kMinRating = 1;
constexpr int kMaxRating = 5;
constexpr int kPositiveRating = 4;

} // namespace

CustomerFeedback::CustomerFeedback(int64_t clientId, int64_t orderId, int rating,
                                   std::string comment, TransportDate submittedOn)
    : clientId_(clientId),
      orderId_(orderId),
      rating_(rating),
      comment_(std::move(comment)),
      submittedOn_(submittedOn) {
    if (clientId_ <= 0 || orderId_ <= 0) {
        throw TransportException("Feedback client and order ids must be positive");
    }
    if (rating_ < kMinRating || rating_ > kMaxRating) {
        throw InvalidRatingException("Feedback rating must be between 1 and 5");
    }
    if (comment_.empty()) {
        throw TransportException("Feedback comment must not be empty");
    }
}

bool CustomerFeedback::isPositive() const noexcept {
    return rating_ >= kPositiveRating;
}

std::string CustomerFeedback::summary() const {
    std::ostringstream out;
    out << "Order #" << orderId_ << ": " << rating_ << "/5 " << comment_;
    return out.str();
}

} // namespace transport
