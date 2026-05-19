#pragma once

#include "routine.h"
#include <string>
#include <vector>

namespace Palette
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

namespace Canvas
{
    void showBanner(const std::string& title);
    void showCaption(const std::string& title);
    void showOption(int num, const std::string& text);

    void showOk(const std::string& msg);
    void showFail(const std::string& msg);
    void showAlert(const std::string& msg);
    void showNote(const std::string& msg);

    void showBar(double percentage, int width = 30);

    void showEntry(const TaskEntry& t);
    void showEntryList(const std::vector<TaskEntry>& entries);
    void drawLine(char ch = '=', int width = 45);

    void wipeScreen();
    int  grabChoice();
    std::string grabInput(const std::string& prompt);
    int  grabInt(const std::string& prompt, int min_val, int max_val);
    void holdForEnter();
}
