#pragma once

#include "routine.h"
#include <string>
#include <vector>

// ============================================================================
//   C++11: constexpr - values computed at compile time (zero runtime cost)
//   ANSI escape codes let us add colors and styles in the terminal
//   Works on Linux, Mac, and Windows 10+ terminals
// ============================================================================
namespace Color
{
    constexpr const char* RESET   = "\033[0m";
    constexpr const char* RED     = "\033[31m";
    constexpr const char* GREEN   = "\033[32m";
    constexpr const char* YELLOW  = "\033[33m";
    constexpr const char* BLUE    = "\033[34m";
    constexpr const char* MAGENTA = "\033[35m";
    constexpr const char* CYAN    = "\033[36m";
    constexpr const char* WHITE   = "\033[37m";
    constexpr const char* BOLD    = "\033[1m";
    constexpr const char* DIM     = "\033[2m";
}

// ============================================================================
// Display: all console output helpers (menus, colors, progress bar, etc.)
// ============================================================================
namespace Display
{
    void printHeader(const std::string& title);
    void printSubHeader(const std::string& title);
    void printMenuOption(int num, const std::string& text);

    void printSuccess(const std::string& msg);
    void printError(const std::string& msg);
    void printWarning(const std::string& msg);
    void printInfo(const std::string& msg);

    // Visual progress bar: [████░░░░░░] 45.0%
    void printProgressBar(double percentage, int width = 30);

    void printRoutine(const Routine& r);
    void printRoutineList(const std::vector<Routine>& routines);
    void printSeparator(char ch = '=', int width = 45);

    void clearScreen();
    int  getChoice();
    std::string getInput(const std::string& prompt);
    int  getIntInput(const std::string& prompt, int min_val, int max_val);
    void pressEnterToContinue();
}
