#pragma once

#include <string>

namespace transport {

class TransportDate {
public:
    TransportDate() = default;
    TransportDate(int day, int month, int year);

    [[nodiscard]] int day() const noexcept { return day_; }
    [[nodiscard]] int month() const noexcept { return month_; }
    [[nodiscard]] int year() const noexcept { return year_; }

    [[nodiscard]] bool isValid() const noexcept;
    [[nodiscard]] bool isBefore(const TransportDate& other) const noexcept;
    [[nodiscard]] bool isAfter(const TransportDate& other) const noexcept;
    [[nodiscard]] bool sameDay(const TransportDate& other) const noexcept;
    [[nodiscard]] TransportDate addDays(int days) const;
    [[nodiscard]] int daysUntil(const TransportDate& other) const;
    [[nodiscard]] std::string toString() const;

private:
    int day_ = 1;
    int month_ = 1;
    int year_ = 1970;

    [[nodiscard]] long long serialDay() const noexcept;
    static TransportDate fromSerialDay(long long serial);
};

} // namespace transport
