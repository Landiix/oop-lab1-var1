#pragma once

#include "Lesson.h"

#include <memory>
#include <string>
#include <vector>

struct ValidationIssue {
    std::string message;
};

class ScheduleRule {
public:
    virtual ~ScheduleRule() = default;

    virtual std::vector<ValidationIssue> check(
        const Lesson& candidate,
        const std::vector<std::unique_ptr<Lesson>>& existing) const = 0;
};

class TeacherAvailabilityRule final : public ScheduleRule {
public:
    std::vector<ValidationIssue> check(
        const Lesson& candidate,
        const std::vector<std::unique_ptr<Lesson>>& existing) const override {
        std::vector<ValidationIssue> issues;
        for (const auto& lesson : existing) {
            if (lesson->teacher() == candidate.teacher() &&
                lesson->interval().overlaps(candidate.interval())) {
                issues.push_back(
                    {"преподаватель занят до " + lesson->interval().end().toString()});
            }
        }
        return issues;
    }
};

class GroupAvailabilityRule final : public ScheduleRule {
public:
    std::vector<ValidationIssue> check(
        const Lesson& candidate,
        const std::vector<std::unique_ptr<Lesson>>& existing) const override {
        std::vector<ValidationIssue> issues;
        for (const auto& lesson : existing) {
            if (lesson->group() == candidate.group() &&
                lesson->interval().overlaps(candidate.interval())) {
                issues.push_back(
                    {"группа " + candidate.group().code() + " занята до " +
                     lesson->interval().end().toString()});
            }
        }
        return issues;
    }
};

class ClassroomAvailabilityRule final : public ScheduleRule {
public:
    std::vector<ValidationIssue> check(
        const Lesson& candidate,
        const std::vector<std::unique_ptr<Lesson>>& existing) const override {
        std::vector<ValidationIssue> issues;
        const Classroom* room = candidate.classroom();
        if (room == nullptr) {
            return issues;
        }

        for (const auto& lesson : existing) {
            const Classroom* other = lesson->classroom();
            if (other != nullptr && *other == *room &&
                lesson->interval().overlaps(candidate.interval())) {
                issues.push_back(
                    {"аудитория " + room->number() + " занята до " +
                     lesson->interval().end().toString()});
            }
        }
        return issues;
    }
};

class FormatTransitionRule final : public ScheduleRule {
public:
    explicit FormatTransitionRule(Duration minGap = Duration::academicPair())
        : minGap_(minGap) {}

    std::vector<ValidationIssue> check(
        const Lesson& candidate,
        const std::vector<std::unique_ptr<Lesson>>& existing) const override {
        std::vector<ValidationIssue> issues;

        for (const auto& lesson : existing) {
            const bool samePeople =
                lesson->teacher() == candidate.teacher() ||
                lesson->group() == candidate.group();
            if (!samePeople) {
                continue;
            }
            if (lesson->format() == candidate.format()) {
                continue;
            }
            if (lesson->interval().date() != candidate.interval().date()) {
                continue;
            }

            bool violated = lesson->interval().overlaps(candidate.interval());

            Duration gap(0);
            if (lesson->interval().gapBefore(candidate.interval(), gap) &&
                gap < minGap_) {
                violated = true;
            }
            if (candidate.interval().gapBefore(lesson->interval(), gap) &&
                gap < minGap_) {
                violated = true;
            }

            if (violated) {
                const char* kind =
                    candidate.format() == LessonFormat::Online ? "дистанционным"
                                                               : "очным";
                issues.push_back(
                    {std::string("перед ") + kind +
                     " занятием требуется свободное окно продолжительностью не "
                     "менее одной учебной пары"});
                break;
            }
        }
        return issues;
    }

private:
    Duration minGap_;
};
