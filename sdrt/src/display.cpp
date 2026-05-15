#include "display.h"
#include <iostream>
#include <iomanip>   // for std::setw, std::setprecision, std::fixed
#include <limits>    // for std::numeric_limits (input validation)

namespace Display
{

// ============================================================================
// Print a bold, centered header with separators
// ============================================================================
void printHeader(const std::string& title)
{
    std::cout << "\n";
    printSeparator('=', 45);

    // Center the title text
    int padding = (45 - static_cast<int>(title.size())) / 2;
    std::cout << Color::BOLD << Color::CYAN;
    for(int i = 0; i < padding; ++i) std::cout << ' ';
    std::cout << title << Color::RESET << "\n";

    printSeparator('=', 45);
}

void printSubHeader(const std::string& title)
{
    std::cout << "\n  " << Color::BOLD << Color::BLUE
              << title << Color::RESET << "\n";
    std::cout << "  ";
    for(int i = 0; i < 40; ++i) std::cout << '-';
    std::cout << "\n";
}

// ============================================================================
// Menu option: "0. Back" is dimmed, others are green-numbered
// ============================================================================
void printMenuOption(int num, const std::string& text)
{
    if(num == 0)
    {
        std::cout << "  " << Color::DIM << num << ". " << text
                  << Color::RESET << "\n";
    }
    else
    {
        std::cout << "  " << Color::GREEN << num << Color::RESET
                  << ". " << text << "\n";
    }
}

// ============================================================================
// Status messages with icons and colors
// ============================================================================
void printSuccess(const std::string& msg)
{
    std::cout << Color::GREEN << "  [OK] " << msg << Color::RESET << "\n";
}

void printError(const std::string& msg)
{
    std::cout << Color::RED << "  [ERROR] " << msg << Color::RESET << "\n";
}

void printWarning(const std::string& msg)
{
    std::cout << Color::YELLOW << "  [!] " << msg << Color::RESET << "\n";
}

void printInfo(const std::string& msg)
{
    std::cout << Color::CYAN << "  [i] " << msg << Color::RESET << "\n";
}

// ============================================================================
// Progress bar: visual percentage indicator
//
//   Example output:  [████████████░░░░░░░░░░░░░░░░░░] 40.0%
//
//   The bar fills with green blocks proportional to the percentage
// ============================================================================
void printProgressBar(double percentage, int width)
{
    if(percentage < 0.0) percentage = 0.0;
    if(percentage > 100.0) percentage = 100.0;

    int filled = static_cast<int>((percentage / 100.0) * width);

    std::cout << "  [";
    for(int i = 0; i < width; ++i)
    {
        if(i < filled)
            std::cout << Color::GREEN << "#" << Color::RESET;
        else
            std::cout << Color::DIM << "-" << Color::RESET;
    }
    std::cout << "] " << Color::BOLD
              << std::fixed << std::setprecision(1)
              << percentage << "%" << Color::RESET << "\n";
}

// ============================================================================
// Print a single routine with all its details, color-coded
// ============================================================================
void printRoutine(const Routine& r)
{
    // Status indicator
    std::string status = r.completed
        ? (std::string(Color::GREEN) + "[DONE]" + Color::RESET)
        : (std::string(Color::YELLOW) + "[PENDING]" + Color::RESET);

    // Priority color
    const char* pri_color = Color::WHITE;
    switch(r.priority)
    {
        case Priority::LOW:      pri_color = Color::DIM;    break;
        case Priority::MEDIUM:   pri_color = Color::BLUE;   break;
        case Priority::HIGH:     pri_color = Color::YELLOW; break;
        case Priority::CRITICAL: pri_color = Color::RED;    break;
    }

    std::cout << "  " << Color::BOLD << "#" << r.id << Color::RESET
              << "  " << status
              << "  " << pri_color << Routine::priorityToString(r.priority)
              << Color::RESET << "\n";

    std::cout << "     " << Color::BOLD << r.name << Color::RESET << "\n";

    if(!r.description.empty())
        std::cout << "     " << Color::DIM << r.description << Color::RESET << "\n";

    if(!r.category.empty())
        std::cout << "     Category: " << r.category << "\n";

    std::cout << "     Created: " << r.created_at << "\n";

    if(r.completed && !r.completed_at.empty())
        std::cout << "     Completed: " << r.completed_at << "\n";

    // C++17: optional::has_value() - check before accessing
    if(r.reminder_time.has_value())
        std::cout << "     Reminder: " << r.reminder_time.value() << "\n";

    std::cout << "\n";
}

// ============================================================================
// Print a list of routines (or "no routines" message)
// ============================================================================
void printRoutineList(const std::vector<Routine>& routines)
{
    if(routines.empty())
    {
        printInfo("No routines found.");
        return;
    }

    // C++11: Range-based for loop
    for(const auto& r : routines)
    {
        printRoutine(r);
    }
}

void printSeparator(char ch, int width)
{
    std::cout << Color::DIM;
    for(int i = 0; i < width; ++i)
    {
        std::cout << ch;
    }
    std::cout << Color::RESET << "\n";
}

// ============================================================================
// Clear the terminal screen (ANSI escape sequence)
// ============================================================================
void clearScreen()
{
    std::cout << "\033[2J\033[H";
}

// ============================================================================
// Input helpers with validation
// ============================================================================
int getChoice()
{
    std::cout << "\n  " << Color::CYAN << "Enter Choice: " << Color::RESET;
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

std::string getInput(const std::string& prompt)
{
    std::cout << "  " << Color::CYAN << prompt << ": " << Color::RESET;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int getIntInput(const std::string& prompt, int min_val, int max_val)
{
    while(true)
    {
        std::cout << "  " << Color::CYAN << prompt
                  << " (" << min_val << "-" << max_val << "): " << Color::RESET;
        int val;
        if(std::cin >> val && val >= min_val && val <= max_val)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printError("Invalid input. Please try again.");
    }
}

void pressEnterToContinue()
{
    std::cout << "\n  " << Color::DIM << "Press Enter to continue..."
              << Color::RESET;
    std::cin.get();
}

}  // namespace Display
