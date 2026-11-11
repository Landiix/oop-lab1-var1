#include "Lesson.h"
#include "Schedule.h"
#include "TimeTypes.h"

#include "Test.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main() {
    std::cout << "Задание 1. Расписание — Усманов Максим, ИДБ-25-01, вариант 1\n\n";

    //TimeInterval
    TimeInterval a(Date(1, 9), TimeOfDay(12, 20), Duration::academicPair());
    TimeInterval b(Date(1, 9), TimeOfDay(13, 0), Duration(60));
    TimeInterval c(Date(1, 9), TimeOfDay(15, 50), Duration::academicPair());

    check(!(a & b).empty(), "пересечение пересекающихся интервалов");
    check((a & c).empty(), "нет пересечения у далёких интервалов");
    check(a == a && a != c, "операторы == и !=");
    check(a.end().toString() == "13:55", "конец пары 12:20 = 13:55");

    Duration d = Duration::academicPair();
    Duration e = d + Duration(5);
    check(d.minutes() == 95 && e.minutes() == 100, "Duration иммутабелен");

    //иерархия занятий
    OfflineLesson offline(
        "ООП",
        Teacher("Иванов И.И."),
        StudyGroup("ИДБ-25-01"),
        academicPairAt(Date(2, 9), TimeOfDay(12, 20)),
        Activity::Lecture,
        Classroom("А-101"));
    OnlineLesson online(
        "ООП",
        Teacher("Иванов И.И."),
        StudyGroup("ИДБ-25-01"),
        academicPairAt(Date(2, 9), TimeOfDay(15, 50)),
        Activity::Practice);

    check(offline.classroom() != nullptr &&
              offline.classroom()->number() == "А-101",
          "очное занятие имеет аудиторию");
    check(online.classroom() == nullptr, "дистанционное занятие без аудитории");

    Schedule schedule;
    std::cout << schedule.tryAdd(std::make_unique<OfflineLesson>(
                    "Математика",
                    Teacher("Иванов И.И."),
                    StudyGroup("ИДБ-98-01"),
                    academicPairAt(Date(1, 9), TimeOfDay(12, 20)),
                    Activity::Lecture,
                    Classroom("Б-201")))
              << "\n\n";

    const std::string conflict = schedule.tryAdd(std::make_unique<OnlineLesson>(
        "ООП",
        Teacher("Иванов И.И."),
        StudyGroup("ИДБ-98-01"),
        academicPairAt(Date(1, 9), TimeOfDay(12, 20)),
        Activity::Lab));
    std::cout << conflict << "\n\n";

    check(conflict.find("15:50") != std::string::npos, "рекомендация 15:50");
    check(schedule.size() == 2, "после переноса в расписании 2 занятия");

    std::cout << schedule.tryAdd(std::make_unique<OfflineLesson>(
                    "ООП",
                    Teacher("Иванов И.И."),
                    StudyGroup("ИДБ-98-02"),
                    academicPairAt(Date(2, 9), TimeOfDay(12, 20)),
                    Activity::Practice,
                    Classroom("Б-202")))
              << "\n";

    std::vector<std::unique_ptr<Lesson>> lessons;
    lessons.push_back(std::make_unique<OfflineLesson>(
        "A",
        Teacher("T"),
        StudyGroup("G"),
        academicPairAt(Date(3, 9), TimeOfDay(8, 30)),
        Activity::Lecture,
        Classroom("1")));
    lessons.push_back(std::make_unique<OnlineLesson>(
        "B",
        Teacher("T"),
        StudyGroup("G"),
        academicPairAt(Date(3, 9), TimeOfDay(12, 20)),
        Activity::Lab));
    check(lessons[0]->format() != lessons[1]->format(),
          "работа через базовый тип Lesson");

    printSummary("task1");
    return g_failed == 0 ? 0 : 1;
}
