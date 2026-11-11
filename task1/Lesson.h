#pragma once

#include "Domain.h"
#include "TimeTypes.h"

#include <memory>
#include <string>
#include <utility>

class Lesson {
public:
    Lesson(std::string subject,
           Teacher teacher,
           StudyGroup group,
           TimeInterval interval,
           Activity activity)
        : subject_(std::move(subject)),
          teacher_(std::move(teacher)),
          group_(std::move(group)),
          interval_(interval),
          activity_(activity) {}

    virtual ~Lesson() = default;

    const std::string& subject() const { return subject_; }
    const Teacher& teacher() const { return teacher_; }
    const StudyGroup& group() const { return group_; }
    const TimeInterval& interval() const { return interval_; }
    Activity activity() const { return activity_; }

    virtual LessonFormat format() const = 0;

    virtual const Classroom* classroom() const = 0;

    virtual std::unique_ptr<Lesson> cloneWithStart(TimeOfDay newStart) const = 0;

protected:
    std::string subject_;
    Teacher teacher_;
    StudyGroup group_;
    TimeInterval interval_;
    Activity activity_;
};

class OfflineLesson final : public Lesson {
public:
    OfflineLesson(std::string subject,
                  Teacher teacher,
                  StudyGroup group,
                  TimeInterval interval,
                  Activity activity,
                  Classroom classroom)
        : Lesson(std::move(subject),
                 std::move(teacher),
                 std::move(group),
                 interval,
                 activity),
          classroom_(std::move(classroom)) {}

    LessonFormat format() const override { return LessonFormat::Offline; }
    const Classroom* classroom() const override { return &classroom_; }

    std::unique_ptr<Lesson> cloneWithStart(TimeOfDay newStart) const override {
        return std::make_unique<OfflineLesson>(
            subject_,
            teacher_,
            group_,
            TimeInterval(interval_.date(), newStart, interval_.duration()),
            activity_,
            classroom_);
    }

private:
    Classroom classroom_;
};

class OnlineLesson final : public Lesson {
public:
    OnlineLesson(std::string subject,
                 Teacher teacher,
                 StudyGroup group,
                 TimeInterval interval,
                 Activity activity)
        : Lesson(std::move(subject),
                 std::move(teacher),
                 std::move(group),
                 interval,
                 activity) {}

    LessonFormat format() const override { return LessonFormat::Online; }
    const Classroom* classroom() const override { return nullptr; }

    std::unique_ptr<Lesson> cloneWithStart(TimeOfDay newStart) const override {
        return std::make_unique<OnlineLesson>(
            subject_,
            teacher_,
            group_,
            TimeInterval(interval_.date(), newStart, interval_.duration()),
            activity_);
    }
};
