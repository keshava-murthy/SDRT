#include "routine.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

TaskEntry::TaskEntry()
    : id(0)
    , urgency(Urgency::MEDIUM)
    , finished(false)
    , nudge_interval_hrs(24)
{
}

TaskEntry::TaskEntry(int id, std::string name, std::string desc,
                     std::string cat, Urgency urg, int freq_hours)
    : id(id)
    , name(std::move(name))
    , description(std::move(desc))
    , category(std::move(cat))
    , urgency(urg)
    , finished(false)
    , born_at(stampNow())
    , nudge_interval_hrs(freq_hours)
{
}

std::string TaskEntry::urgencyLabel(Urgency u)
{
    switch(u)
    {
        case Urgency::LOW:      return "Low";
        case Urgency::MEDIUM:   return "Medium";
        case Urgency::HIGH:     return "High";
        case Urgency::CRITICAL: return "Critical";
        default:                return "Unknown";
    }
}

Urgency TaskEntry::parseUrgency(const std::string& s)
{
    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if(lower == "low"      || lower == "1") return Urgency::LOW;
    if(lower == "high"     || lower == "3") return Urgency::HIGH;
    if(lower == "critical" || lower == "4") return Urgency::CRITICAL;
    return Urgency::MEDIUM;
}

std::string TaskEntry::stampNow()
{
    auto now  = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
