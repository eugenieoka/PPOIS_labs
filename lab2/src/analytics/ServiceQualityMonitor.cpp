#include "transport/analytics/ServiceQualityMonitor.h"

#include "transport/Constants.h"

namespace transport {

void ServiceQualityMonitor::addFeedback(const CustomerFeedback& feedback) {
    feedbacks_.push_back(feedback);
}

double ServiceQualityMonitor::averageRating() const noexcept {
    if (feedbacks_.empty()) {
        return 0.0;
    }
    double total = 0.0;
    for (const CustomerFeedback& feedback : feedbacks_) {
        total += static_cast<double>(feedback.rating());
    }
    return total / static_cast<double>(feedbacks_.size());
}

double ServiceQualityMonitor::complaintRate() const noexcept {
    if (feedbacks_.empty()) {
        return 0.0;
    }
    std::size_t complaints = 0;
    for (const CustomerFeedback& feedback : feedbacks_) {
        if (feedback.rating() <= constants::kComplaintRatingThreshold) {
            ++complaints;
        }
    }
    return static_cast<double>(complaints) / static_cast<double>(feedbacks_.size());
}

std::vector<std::string> ServiceQualityMonitor::topComplaints(std::size_t limit) const {
    std::vector<std::string> result;
    for (const CustomerFeedback& feedback : feedbacks_) {
        if (result.size() >= limit) {
            break;
        }
        if (feedback.rating() <= constants::kComplaintRatingThreshold) {
            result.push_back(feedback.comment());
        }
    }
    return result;
}

} // namespace transport
