#pragma once

#include "Lesson.h"
#include "ScheduleRule.h"

#include <memory>
#include <string>
#include <vector>

class Schedule {
public:
    Schedule();

    void addRule(std::unique_ptr<ScheduleRule> rule);

    std::vector<ValidationIssue> validate(const Lesson& candidate) const;

    bool findNearestStart(const Lesson& prototype,
                          TimeOfDay fromInclusive,
                          TimeOfDay& outStart) const;

    std::string tryAdd(std::unique_ptr<Lesson> lesson);

    std::size_t size() const { return lessons_.size(); }
    const std::vector<std::unique_ptr<Lesson>>& lessons() const { return lessons_; }

private:
    std::vector<std::unique_ptr<Lesson>> lessons_;
    std::vector<std::unique_ptr<ScheduleRule>> rules_;
};
