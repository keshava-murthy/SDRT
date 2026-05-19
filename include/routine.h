#pragma once

#include <string>
#include <optional>
#include <chrono>

enum class Urgency
{
    LOW      = 1,
    MEDIUM   = 2,
    HIGH     = 3,
    CRITICAL = 4
};

struct TaskEntry
{
    int id;
    std::string name;
    std::string description;
    std::string category;
    Urgency urgency;
    bool finished;
    std::string born_at;
    std::string sealed_at;
    std::optional<std::string> alert_moment;
    int nudge_interval_hrs;

    TaskEntry();

    TaskEntry(int id, std::string name, std::string desc,
              std::string cat, Urgency urg, int freq_hours = 24);

    static std::string urgencyLabel(Urgency u);
    static Urgency parseUrgency(const std::string& s);
    static std::string stampNow();
};
