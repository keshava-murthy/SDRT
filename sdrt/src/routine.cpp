#include "routine.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

// ============================================================================
// Default constructor: initializes a blank routine
// ============================================================================
Routine::Routine()
    : id(0)
    , priority(Priority::MEDIUM)
    , completed(false)
    , notification_freq_hours(24)
{
}

// ============================================================================
// Parameterized constructor
//   C++11: std::move() transfers ownership of the string data
//     Instead of copying "Exercise for 30 min" character by character,
//     move just swaps internal pointers — almost free!
// ============================================================================
Routine::Routine(int id, std::string name, std::string desc,
                 std::string cat, Priority pri, int freq_hours)
    : id(id)
    , name(std::move(name))           // Move: no copy, just pointer swap
    , description(std::move(desc))    // Move: efficient transfer
    , category(std::move(cat))        // Move: avoids deep copy
    , priority(pri)
    , completed(false)
    , created_at(currentTimeStr())
    , notification_freq_hours(freq_hours)
{
}

// ============================================================================
// Convert Priority enum to a human-readable string
// ============================================================================
std::string Routine::priorityToString(Priority p)
{
    switch(p)
    {
        case Priority::LOW:      return "Low";
        case Priority::MEDIUM:   return "Medium";
        case Priority::HIGH:     return "High";
        case Priority::CRITICAL: return "Critical";
        default:                 return "Unknown";
    }
}

// ============================================================================
// Parse a string (or number) into a Priority enum
// ============================================================================
Priority Routine::stringToPriority(const std::string& s)
{
    std::string lower = s;
    // C++11: lambda passed to std::transform
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if(lower == "low"      || lower == "1") return Priority::LOW;
    if(lower == "high"     || lower == "3") return Priority::HIGH;
    if(lower == "critical" || lower == "4") return Priority::CRITICAL;
    return Priority::MEDIUM;  // safe default
}

// ============================================================================
// Get current timestamp as "YYYY-MM-DD HH:MM:SS"
//   C++11: std::chrono for high-precision time
// ============================================================================
std::string Routine::currentTimeStr()
{
    auto now  = std::chrono::system_clock::now();             // current time point
    auto time = std::chrono::system_clock::to_time_t(now);   // convert to time_t
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
