#pragma once

#include <string>
#include <optional>  // C++17: represents a value that may or may not exist
#include <chrono>    // C++11: time handling utilities

// ============================================================================
//   C++11: enum class (scoped enumeration)
//   - Type-safe: won't implicitly convert to int
//   - Scoped: must use Priority::LOW, not just LOW
// ============================================================================
enum class Priority
{
    LOW      = 1,
    MEDIUM   = 2,
    HIGH     = 3,
    CRITICAL = 4
};

// ============================================================================
// Routine: represents a single task/activity in the tracker
// ============================================================================
struct Routine
{
    int id;
    std::string name;
    std::string description;
    std::string category;
    Priority priority;
    bool completed;
    std::string created_at;
    std::string completed_at;

    // C++17: std::optional - cleanly says "this value might not be set"
    //   Instead of using magic values like "" or -1, optional is self-documenting
    std::optional<std::string> reminder_time;

    int notification_freq_hours;  // how often to remind (default: 24)

    // Default constructor
    Routine();

    //   C++11: Move semantics - strings are moved (transferred) instead of copied
    //   This avoids expensive deep copies of string data
    Routine(int id, std::string name, std::string desc,
            std::string cat, Priority pri, int freq_hours = 24);

    // Utility: convert between Priority enum and readable strings
    static std::string priorityToString(Priority p);
    static Priority stringToPriority(const std::string& s);

    // Get current date-time as a readable string
    static std::string currentTimeStr();
};
