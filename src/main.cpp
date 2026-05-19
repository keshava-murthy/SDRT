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
//     - Function Wrappers: std::function
//
//   Day 2 Topics:
//     - Lambda Captures and Callbacks
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
//     - std::chrono (time handling)
//
//   Future:
//     - Reminder Service with threading (std::thread, mutex, etc.)
//
// ============================================================================

#include "routine_manager.h"
#include "statistics.h"
#include "data_manager.h"
#include "menu.h"

#include <memory>     // C++11: smart pointers (unique_ptr, shared_ptr)
#include <iostream>

int main()
{
    // Using unique_ptr for automatic memory management.
    // When these go out of scope, memory is freed automatically - no manual delete needed.
    auto manager  = std::make_unique<RoutineManager>();
    auto stats    = std::make_unique<Statistics>(*manager);
    auto data_mgr = std::make_unique<DataManager>("data/routines.json");

    // Create the menu and pass references to all core components
    Menu menu(*manager, *stats, *data_mgr);

    // Start the interactive menu loop
    menu.run();

    return 0;
}
