#pragma once

#include <string>
#include <utility>

enum class Activity {
    Lecture,
    Practice,
    Lab
};

enum class LessonFormat {
    Offline,
    Online
};

class Teacher {
public:
    explicit Teacher(std::string name) : name_(std::move(name)) {}

    const std::string& name() const { return name_; }

    bool operator==(const Teacher& other) const { return name_ == other.name_; }
    bool operator!=(const Teacher& other) const { return !(*this == other); }

private:
    std::string name_;
};

class StudyGroup {
public:
    explicit StudyGroup(std::string code) : code_(std::move(code)) {}

    const std::string& code() const { return code_; }

    bool operator==(const StudyGroup& other) const { return code_ == other.code_; }
    bool operator!=(const StudyGroup& other) const { return !(*this == other); }

private:
    std::string code_;
};

class Classroom {
public:
    explicit Classroom(std::string number) : number_(std::move(number)) {}

    const std::string& number() const { return number_; }

    bool operator==(const Classroom& other) const { return number_ == other.number_; }
    bool operator!=(const Classroom& other) const { return !(*this == other); }

private:
    std::string number_;
};
