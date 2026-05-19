#include "display.h"
#include <iostream>
#include <iomanip>
#include <limits>

namespace Canvas
{

void showBanner(const std::string& title)
{
    std::cout << "\n";
    drawLine('=', 45);

    int padding = (45 - static_cast<int>(title.size())) / 2;
    std::cout << Palette::BOLD << Palette::CYAN;
    for(int i = 0; i < padding; ++i) std::cout << ' ';
    std::cout << title << Palette::RESET << "\n";

    drawLine('=', 45);
}

void showCaption(const std::string& title)
{
    std::cout << "\n  " << Palette::BOLD << Palette::BLUE
              << title << Palette::RESET << "\n";
    std::cout << "  ";
    for(int i = 0; i < 40; ++i) std::cout << '-';
    std::cout << "\n";
}

void showOption(int num, const std::string& text)
{
    if(num == 0)
    {
        std::cout << "  " << Palette::DIM << num << ". " << text
                  << Palette::RESET << "\n";
    }
    else
    {
        std::cout << "  " << Palette::GREEN << num << Palette::RESET
                  << ". " << text << "\n";
    }
}

void showOk(const std::string& msg)
{
    std::cout << Palette::GREEN << "  [OK] " << msg << Palette::RESET << "\n";
}

void showFail(const std::string& msg)
{
    std::cout << Palette::RED << "  [ERROR] " << msg << Palette::RESET << "\n";
}

void showAlert(const std::string& msg)
{
    std::cout << Palette::YELLOW << "  [!] " << msg << Palette::RESET << "\n";
}

void showNote(const std::string& msg)
{
    std::cout << Palette::CYAN << "  [i] " << msg << Palette::RESET << "\n";
}

void showBar(double percentage, int width)
{
    if(percentage < 0.0) percentage = 0.0;
    if(percentage > 100.0) percentage = 100.0;

    int filled = static_cast<int>((percentage / 100.0) * width);

    std::cout << "  [";
    for(int i = 0; i < width; ++i)
    {
        if(i < filled)
            std::cout << Palette::GREEN << "#" << Palette::RESET;
        else
            std::cout << Palette::DIM << "-" << Palette::RESET;
    }
    std::cout << "] " << Palette::BOLD
              << std::fixed << std::setprecision(1)
              << percentage << "%" << Palette::RESET << "\n";
}

void showEntry(const TaskEntry& t)
{
    std::string status = t.finished
        ? (std::string(Palette::GREEN) + "[DONE]" + Palette::RESET)
        : (std::string(Palette::YELLOW) + "[PENDING]" + Palette::RESET);

    const char* urg_color = Palette::WHITE;
    switch(t.urgency)
    {
        case Urgency::LOW:      urg_color = Palette::DIM;    break;
        case Urgency::MEDIUM:   urg_color = Palette::BLUE;   break;
        case Urgency::HIGH:     urg_color = Palette::YELLOW; break;
        case Urgency::CRITICAL: urg_color = Palette::RED;    break;
    }

    std::cout << "  " << Palette::BOLD << "#" << t.id << Palette::RESET
              << "  " << status
              << "  " << urg_color << TaskEntry::urgencyLabel(t.urgency)
              << Palette::RESET << "\n";

    std::cout << "     " << Palette::BOLD << t.name << Palette::RESET << "\n";

    if(!t.description.empty())
        std::cout << "     " << Palette::DIM << t.description << Palette::RESET << "\n";

    if(!t.category.empty())
        std::cout << "     Category: " << t.category << "\n";

    std::cout << "     Created: " << t.born_at << "\n";

    if(t.finished && !t.sealed_at.empty())
        std::cout << "     Completed: " << t.sealed_at << "\n";

    if(t.alert_moment.has_value())
        std::cout << "     Reminder: " << t.alert_moment.value() << "\n";

    std::cout << "\n";
}

void showEntryList(const std::vector<TaskEntry>& entries)
{
    if(entries.empty())
    {
        showNote("No routines found.");
        return;
    }

    for(const auto& e : entries)
        showEntry(e);
}

void drawLine(char ch, int width)
{
    std::cout << Palette::DIM;
    for(int i = 0; i < width; ++i)
        std::cout << ch;
    std::cout << Palette::RESET << "\n";
}

void wipeScreen()
{
    std::cout << "\033[2J\033[H";
}

int grabChoice()
{
    std::cout << "\n  " << Palette::CYAN << "Enter Choice: " << Palette::RESET;
    int choice;
    if(!(std::cin >> choice))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

std::string grabInput(const std::string& prompt)
{
    std::cout << "  " << Palette::CYAN << prompt << ": " << Palette::RESET;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int grabInt(const std::string& prompt, int min_val, int max_val)
{
    while(true)
    {
        std::cout << "  " << Palette::CYAN << prompt
                  << " (" << min_val << "-" << max_val << "): " << Palette::RESET;
        int val;
        if(std::cin >> val && val >= min_val && val <= max_val)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        showFail("Invalid input. Please try again.");
    }
}

void holdForEnter()
{
    std::cout << "\n  " << Palette::DIM << "Press Enter to continue..."
              << Palette::RESET;
    std::cin.get();
}

}
