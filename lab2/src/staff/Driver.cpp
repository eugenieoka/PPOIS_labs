#include "transport/staff/Driver.h"

#include "transport/Constants.h"
#include "transport/exceptions/DriverNotAvailableException.h"
#include "transport/exceptions/InvalidOrderStateException.h"
#include "transport/exceptions/InvalidRatingException.h"
#include "transport/exceptions/TransportException.h"

#include <utility>

namespace transport {
namespace {

constexpr int kMaxExperienceYears = 60;

} // namespace

Driver::Driver(int64_t employeeId, std::string fullName, Address address, std::string phone,
               Salary salary, TransportDate hireDate, DrivingLicense license, int experienceYears)
    : Employee(employeeId, std::move(fullName), std::move(address), std::move(phone),
               std::move(salary), hireDate),
      license_(std::move(license)),
      experienceYears_(experienceYears) {
    if (experienceYears_ < 0 || experienceYears_ > kMaxExperienceYears) {
        throw TransportException("Driver experience is out of range");
    }
}

bool Driver::canDrive(const std::string& category) const noexcept {
    return license_.coversCategory(category);
}

void Driver::assign() {
    if (!available_) {
        throw DriverNotAvailableException("Driver is already assigned to a delivery");
    }
    available_ = false;
}

void Driver::completeDelivery() {
    if (available_) {
        throw InvalidOrderStateException("Driver has no active delivery");
    }
    ++completedDeliveries_;
    available_ = true;
}

void Driver::updateRating(double rating) {
    if (rating < constants::kMinimumDriverRating || rating > constants::kMaxDriverRating) {
        throw InvalidRatingException("Driver rating must be between 0 and 5");
    }
    rating_ = rating;
}

std::string Driver::roleName() const {
    return "Driver";
}

} // namespace transport
