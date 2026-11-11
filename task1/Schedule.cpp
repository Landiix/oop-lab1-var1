#include "Schedule.h"

Schedule::Schedule() {
    rules_.push_back(std::make_unique<TeacherAvailabilityRule>());
    rules_.push_back(std::make_unique<GroupAvailabilityRule>());
    rules_.push_back(std::make_unique<ClassroomAvailabilityRule>());
    rules_.push_back(std::make_unique<FormatTransitionRule>());
}

void Schedule::addRule(std::unique_ptr<ScheduleRule> rule) {
    rules_.push_back(std::move(rule));
}

std::vector<ValidationIssue> Schedule::validate(const Lesson& candidate) const {
    std::vector<ValidationIssue> all;
    for (const auto& rule : rules_) {
        auto part = rule->check(candidate, lessons_);
        all.insert(all.end(), part.begin(), part.end());
    }
    return all;
}

bool Schedule::findNearestStart(const Lesson& prototype,
                                TimeOfDay fromInclusive,
                                TimeOfDay& outStart) const {
    static const int kPairStarts[] = {
        8 * 60 + 30,
        10 * 60 + 10,
        12 * 60 + 20,
        14 * 60,
        15 * 60 + 50,
        17 * 60 + 40,
        19 * 60 + 30,
    };

    for (int startMin : kPairStarts) {
        if (startMin < fromInclusive.asMinutes()) {
            continue;
        }
        auto trial = prototype.cloneWithStart(TimeOfDay::fromMinutes(startMin));
        if (validate(*trial).empty()) {
            outStart = TimeOfDay::fromMinutes(startMin);
            return true;
        }
    }
    return false;
}

std::string Schedule::tryAdd(std::unique_ptr<Lesson> lesson) {
    const auto issues = validate(*lesson);

    if (issues.empty()) {
        std::string message =
            "Занятие " + lesson->interval().date().toString() + " в " +
            lesson->interval().start().toString() + " для преподавателя " +
            lesson->teacher().name() + " и группы " + lesson->group().code() +
            " успешно добавлено.";
        lessons_.push_back(std::move(lesson));
        return message;
    }

    std::string message =
        "Невозможно добавить занятие " + lesson->interval().date().toString() +
        " в " + lesson->interval().start().toString() + " для преподавателя " +
        lesson->teacher().name() + " и группы " + lesson->group().code() +
        ".\nПричины:\n";
    for (const auto& issue : issues) {
        message += "- " + issue.message + "\n";
    }

    TimeOfDay recommended;
    if (findNearestStart(*lesson, lesson->interval().start(), recommended)) {
        message += "Рекомендуемое время начала: " + recommended.toString() +
                   ". Изменение принято.";
        lessons_.push_back(lesson->cloneWithStart(recommended));
    } else {
        message += "Подходящее время не найдено.";
    }
    return message;
}
