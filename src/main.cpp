// ============================================================================
// Smart Daily Routine Tracker (SDRT)
// A Modern C++ Console-Based Productivity System
//
// C++ Features Demonstrated in this project:
//
//   Day 1 Topics:
//     - STL Containers: vector, map, unordered_map
//     - std::pair (quotes benefit messages)
//     - constexpr (ANSI color codes)
//     - Lambda Expressions (filtering, callbacks)
//     - Function Wrappers: std::function (ReminderCallback)
//
//   Day 2 Topics:
//     - Lambda Captures and Callbacks (reminder callbacks)
//     - std::initializer_list (vector initialization with {})
//     - RValue References & Move Semantics (string transfers)
//     - Move Assignment (vector moves in data loading)
//
//   Day 3 Topics:
//     - Range-based for loops (everywhere)
//     - enum class (Priority)
//
//   Day 4 Topics:
//     - Smart Pointers: unique_ptr (main.cpp ownership)
//     - Structured Bindings (map iteration in reports)
//     - std::optional (getRoutineById, reminder_time)
//     - std::chrono (time handling, thread sleep)
//
//   Day 5 Topics:
//     - std::thread (ReminderService background thread)
//     - std::mutex, std::lock_guard (thread safety)
//     - std::condition_variable (interruptible sleep)
//     - std::atomic (thread-safe running flag)
//
// ============================================================================

#include "routine_manager.h"
#include "reminder_service.h"
#include "statistics.h"
#include "data_manager.h"
#include "menu.h"

#include <memory>     // C++11: smart pointers (unique_ptr, shared_ptr)
#include <iostream>

int main()
{
    // C++11: std::unique_ptr - automatic memory management
    //   When these go out of scope, memory is freed automatically.
    //   No manual 'delete' needed - prevents memory leaks!
    auto manager  = std::make_unique<RoutineManager>();
    auto reminder = std::make_unique<ReminderService>(*manager);
    auto stats    = std::make_unique<Statistics>(*manager);
    auto data_mgr = std::make_unique<DataManager>("data/routines.json");

    // Create the menu, passing references to all components
    Menu menu(*manager, *reminder, *stats, *data_mgr);

    // Run the application (enters the interactive menu loop)
    menu.run();

    return 0;
}
