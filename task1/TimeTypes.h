#pragma once

#include <algorithm>
#include <cstdio>
#include <stdexcept>
#include <string>

class Duration {
public:
    explicit Duration(int minutes) : minutes_(minutes) {
        if (minutes < 0) {
            throw std::invalid_argument("Duration: отрицательная длительность");
        }
    }

    static Duration academicPair() { return Duration(95); }

    int minutes() const { return minutes_; }

    Duration operator+(Duration other) const {
        return Duration(minutes_ + other.minutes_);
    }

    bool operator==(Duration other) const { return minutes_ == other.minutes_; }
    bool operator!=(Duration other) const { return !(*this == other); }
    bool operator<(Duration other) const { return minutes_ < other.minutes_; }
    bool operator<=(Duration other) const { return minutes_ <= other.minutes_; }
    bool operator>(Duration other) const { return minutes_ > other.minutes_; }
    bool operator>=(Duration other) const { return minutes_ >= other.minutes_; }

private:
    int minutes_;
};

class TimeOfDay {
public:
    TimeOfDay(int hours = 0, int minutes = 0)
        : totalMinutes_(hours * 60 + minutes) {
        if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
            throw std::invalid_argument("TimeOfDay: некорректное время");
        }
    }

    static TimeOfDay fromMinutes(int total) {
        return TimeOfDay(total / 60, total % 60);
    }

    int hours() const { return totalMinutes_ / 60; }
    int minutes() const { return totalMinutes_ % 60; }
    int asMinutes() const { return totalMinutes_; }

    TimeOfDay operator+(Duration duration) const {
        return fromMinutes(totalMinutes_ + duration.minutes());
    }

    Duration operator-(TimeOfDay other) const {
        return Duration(totalMinutes_ - other.totalMinutes_);
    }

    bool operator==(TimeOfDay other) const {
        return totalMinutes_ == other.totalMinutes_;
    }
    bool operator!=(TimeOfDay other) const { return !(*this == other); }
    bool operator<(TimeOfDay other) const {
        return totalMinutes_ < other.totalMinutes_;
    }
    bool operator<=(TimeOfDay other) const {
        return totalMinutes_ <= other.totalMinutes_;
    }
    bool operator>(TimeOfDay other) const {
        return totalMinutes_ > other.totalMinutes_;
    }
    bool operator>=(TimeOfDay other) const {
        return totalMinutes_ >= other.totalMinutes_;
    }

    std::string toString() const {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "%02d:%02d", hours(), minutes());
        return buf;
    }

private:
    int totalMinutes_;
};

class Date {
public:
    Date(int day = 1, int month = 1) : day_(day), month_(month) {
        if (day < 1 || day > 31 || month < 1 || month > 12) {
            throw std::invalid_argument("Date: некорректная дата");
        }
    }

    int day() const { return day_; }
    int month() const { return month_; }

    bool operator==(Date other) const {
        return day_ == other.day_ && month_ == other.month_;
    }
    bool operator!=(Date other) const { return !(*this == other); }

    std::string toString() const {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "%02d.%02d", day_, month_);
        return buf;
    }

private:
    int day_;
    int month_;
};

class TimeInterval {
public:
    TimeInterval(Date date, TimeOfDay start, Duration duration)
        : date_(date), start_(start), duration_(duration) {}

    Date date() const { return date_; }
    TimeOfDay start() const { return start_; }
    Duration duration() const { return duration_; }
    TimeOfDay end() const { return start_ + duration_; }
    bool empty() const { return duration_.minutes() == 0; }

    TimeInterval operator&(const TimeInterval& other) const {
        if (date_ != other.date_) {
            return TimeInterval(date_, start_, Duration(0));
        }

        const int left = std::max(start_.asMinutes(), other.start_.asMinutes());
        const int right = std::min(end().asMinutes(), other.end().asMinutes());
        if (left >= right) {
            return TimeInterval(date_, start_, Duration(0));
        }
        return TimeInterval(date_, TimeOfDay::fromMinutes(left), Duration(right - left));
    }

    bool overlaps(const TimeInterval& other) const {
        return !(*this & other).empty();
    }

    bool gapBefore(const TimeInterval& next, Duration& gap) const {
        if (date_ != next.date_ || next.start_ < end()) {
            return false;
        }
        gap = next.start_ - end();
        return true;
    }

    bool operator==(const TimeInterval& other) const {
        return date_ == other.date_ && start_ == other.start_ &&
               duration_ == other.duration_;
    }
    bool operator!=(const TimeInterval& other) const { return !(*this == other); }

    std::string toString() const {
        return date_.toString() + " " + start_.toString() + "-" + end().toString();
    }

private:
    Date date_;
    TimeOfDay start_;
    Duration duration_;
};

inline TimeInterval academicPairAt(Date date, TimeOfDay start) {
    return TimeInterval(date, start, Duration::academicPair());
}
