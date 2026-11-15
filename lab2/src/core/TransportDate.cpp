#include "transport/core/TransportDate.h"

#include "transport/exceptions/InvalidDateException.h"

#include <iomanip>
#include <sstream>

namespace transport {
namespace {

constexpr int kMinYear = 1;
constexpr int kMaxYear = 9999;
constexpr int kDaysInFebruaryLeap = 29;
constexpr int kDaysInFebruary = 28;
constexpr int kMonthsInYear = 12;

bool isLeapYear(int year) noexcept {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int daysInMonth(int month, int year) noexcept {
    switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? kDaysInFebruaryLeap : kDaysInFebruary;
        default:
            return 0;
    }
}

// Howard Hinnant's civil calendar algorithms, days relative to 1970-01-01.
long long daysFromCivil(int year, unsigned month, unsigned day) noexcept {
    year -= month <= 2;
    const int era = (year >= 0 ? year : year - 399) / 400;
    const unsigned yearOfEra = static_cast<unsigned>(year - era * 400);
    const unsigned dayOfYear = (153 * (month + (month > 2 ? -3 : 9)) + 2) / 5 + day - 1;
    const unsigned dayOfEra = yearOfEra * 365 + yearOfEra / 4 - yearOfEra / 100 + dayOfYear;
    return static_cast<long long>(era) * 146097 + static_cast<long long>(dayOfEra) - 719468;
}

void civilFromDays(long long serial, int& year, unsigned& month, unsigned& day) noexcept {
    serial += 719468;
    const long long era = (serial >= 0 ? serial : serial - 146096) / 146097;
    const unsigned dayOfEra = static_cast<unsigned>(serial - era * 146097);
    const unsigned yearOfEra = (dayOfEra - dayOfEra / 1460 + dayOfEra / 36524 - dayOfEra / 146096) / 365;
    const int resultYear = static_cast<int>(yearOfEra) + static_cast<int>(era) * 400;
    const unsigned dayOfYear = dayOfEra - (365 * yearOfEra + yearOfEra / 4 - yearOfEra / 100);
    const unsigned monthIndex = (5 * dayOfYear + 2) / 153;
    day = dayOfYear - (153 * monthIndex + 2) / 5 + 1;
    month = monthIndex < 10 ? monthIndex + 3 : monthIndex - 9;
    year = resultYear + (month <= 2);
}

} // namespace

TransportDate::TransportDate(int day, int month, int year)
    : day_(day), month_(month), year_(year) {
    if (!isValid()) {
        throw InvalidDateException("Invalid calendar date");
    }
}

bool TransportDate::isValid() const noexcept {
    return year_ >= kMinYear && year_ <= kMaxYear &&
           month_ >= 1 && month_ <= kMonthsInYear &&
           day_ >= 1 && day_ <= daysInMonth(month_, year_);
}

bool TransportDate::isBefore(const TransportDate& other) const noexcept {
    return serialDay() < other.serialDay();
}

bool TransportDate::isAfter(const TransportDate& other) const noexcept {
    return serialDay() > other.serialDay();
}

bool TransportDate::sameDay(const TransportDate& other) const noexcept {
    return serialDay() == other.serialDay();
}

TransportDate TransportDate::addDays(int days) const {
    return fromSerialDay(serialDay() + days);
}

int TransportDate::daysUntil(const TransportDate& other) const {
    return static_cast<int>(other.serialDay() - serialDay());
}

std::string TransportDate::toString() const {
    std::ostringstream out;
    out << std::setfill('0') << std::setw(2) << day_ << '.'
        << std::setw(2) << month_ << '.' << year_;
    return out.str();
}

long long TransportDate::serialDay() const noexcept {
    return daysFromCivil(year_, static_cast<unsigned>(month_), static_cast<unsigned>(day_));
}

TransportDate TransportDate::fromSerialDay(long long serial) {
    int year = 0;
    unsigned month = 0;
    unsigned day = 0;
    civilFromDays(serial, year, month, day);
    return TransportDate(static_cast<int>(day), static_cast<int>(month), year);
}

} // namespace transport
