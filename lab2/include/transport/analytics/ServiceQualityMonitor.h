#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "transport/analytics/CustomerFeedback.h"

namespace transport {

class ServiceQualityMonitor {
public:
    void addFeedback(const CustomerFeedback& feedback);
    [[nodiscard]] double averageRating() const noexcept;
    [[nodiscard]] double complaintRate() const noexcept;
    [[nodiscard]] std::vector<std::string> topComplaints(std::size_t limit) const;
    [[nodiscard]] std::size_t feedbackCount() const noexcept { return feedbacks_.size(); }

private:
    std::vector<CustomerFeedback> feedbacks_;
};

} // namespace transport
