#include "menu.h"
#include "display.h"
#include "quotes.h"
#include <iostream>
#include <iomanip>   // for std::setw, std::left
#include <string>

// ============================================================================
// Constructor: store references to all core components
// ============================================================================
Menu::Menu(RoutineManager& mgr, ReminderService& rem,
           Statistics& st, DataManager& dm)
    : manager_(mgr)
    , reminder_(rem)
    , stats_(st)
    , data_mgr_(dm)
{
}

// ============================================================================
// Entry point: show welcome screen, load data, then enter main menu loop
// ============================================================================
void Menu::run()
{
    Display::clearScreen();
    Display::printHeader("SMART DAILY ROUTINE TRACKER");
    Display::printInfo("Welcome! Let's make today productive.");

    // Show a random motivational quote on startup
    std::cout << "\n  " << Color::MAGENTA
              << Quotes::getRandomQuote()
              << Color::RESET << "\n";

    // Try to load previously saved data
    if(data_mgr_.loadData(manager_))
    {
        Display::printSuccess("Loaded saved routines.");

        size_t pending = manager_.pendingCount();
        if(pending > 0)
        {
            Display::printWarning(std::to_string(pending) + " routine(s) still pending!");
        }
    }

    Display::pressEnterToContinue();
    mainMenu();
}

// ============================================================================
// Main menu loop
// ============================================================================
void Menu::mainMenu()
{
    while(true)
    {
        Display::clearScreen();
        Display::printHeader("SMART DAILY ROUTINE TRACKER");

        // Quick stats overview on the main screen
        if(manager_.totalCount() > 0)
        {
            Display::printProgressBar(stats_.completionPercentage());
            std::cout << "  "
                      << Color::GREEN << stats_.completedRoutines() << " done"
                      << Color::RESET << " | "
                      << Color::YELLOW << stats_.pendingRoutines() << " pending"
                      << Color::RESET << " | "
                      << stats_.totalRoutines() << " total\n";
        }

        // Notification badge (shows count of piled-up reminders)
        size_t notif_count = reminder_.notificationCount();
        if(notif_count > 0)
        {
            std::cout << "  " << Color::RED << Color::BOLD
                      << ">> " << notif_count << " notification(s) <<"
                      << Color::RESET << "\n";
        }

        std::cout << "\n";
        Display::printMenuOption(1, "Routine Management");
        Display::printMenuOption(2, "Reminder Management");
        Display::printMenuOption(3, "Statistics & Reports");
        Display::printMenuOption(4, "Data Management");
        Display::printMenuOption(0, "Exit");

        int choice = Display::getChoice();
        switch(choice)
        {
            case 1: routineManagement(); break;
            case 2: reminderManagement(); break;
            case 3: statisticsReports(); break;
            case 4: dataManagement(); break;
            case 0:
                // Auto-save on exit
                if(data_mgr_.saveData(manager_))
                {
                    Display::printSuccess("Data saved automatically.");
                }
                reminder_.stop();
                Display::printInfo("Goodbye! Stay productive!");
                return;
            default:
                Display::printError("Invalid choice.");
        }
    }
}

// ============================================================================
// Routine Management sub-menu
// ============================================================================
void Menu::routineManagement()
{
    while(true)
    {
        Display::clearScreen();
        Display::printHeader("ROUTINE MANAGEMENT");

        Display::printMenuOption(1, "Add New Routine");
        Display::printMenuOption(2, "View All Routines");
        Display::printMenuOption(3, "View Pending Routines");
        Display::printMenuOption(4, "View Completed Routines");
        Display::printMenuOption(5, "Mark Routine as Completed");
        Display::printMenuOption(6, "Update Priority");
        Display::printMenuOption(7, "Categorize Routine");
        Display::printMenuOption(8, "Delete Routine");
        Display::printMenuOption(9, "Search Routine");
        Display::printMenuOption(0, "Back");

        int choice = Display::getChoice();
        switch(choice)
        {
            case 1: addNewRoutine();         break;
            case 2: viewAllRoutines();       break;
            case 3: viewPendingRoutines();   break;
            case 4: viewCompletedRoutines(); break;
            case 5: markRoutineCompleted();  break;
            case 6: updateRoutinePriority(); break;
            case 7: categorizeRoutine();     break;
            case 8: deleteRoutine();         break;
            case 9: searchRoutine();         break;
            case 0: return;
            default: Display::printError("Invalid choice.");
        }
    }
}

// ============================================================================
// Reminder Management sub-menu
// ============================================================================
void Menu::reminderManagement()
{
    while(true)
    {
        Display::clearScreen();
        Display::printHeader("REMINDER MANAGEMENT");

        // Show service status
        std::string status = reminder_.isRunning()
            ? (std::string(Color::GREEN) + "RUNNING" + Color::RESET)
            : (std::string(Color::RED)   + "STOPPED" + Color::RESET);
        std::cout << "  Service Status: " << status << "\n\n";

        Display::printMenuOption(1, "Start Reminder Service");
        Display::printMenuOption(2, "Stop Reminder Service");
        Display::printMenuOption(3, "View Notifications");
        Display::printMenuOption(4, "Clear Notifications");
        Display::printMenuOption(5, "Add Reminder Callback");
        Display::printMenuOption(0, "Back");

        int choice = Display::getChoice();
        switch(choice)
        {
            case 1:
                reminder_.start();
                Display::printSuccess("Reminder service started! (checks every 30s)");
                Display::pressEnterToContinue();
                break;
            case 2:
                reminder_.stop();
                Display::printSuccess("Reminder service stopped.");
                Display::pressEnterToContinue();
                break;
            case 3:
                // Show notifications, then clear them (design: disappear once viewed)
                reminder_.executeNotifications();
                if(reminder_.notificationCount() > 0)
                {
                    reminder_.clearNotifications();
                    Display::printInfo("Notifications cleared after viewing.");
                }
                Display::pressEnterToContinue();
                break;
            case 4:
                reminder_.clearNotifications();
                Display::printSuccess("All notifications cleared.");
                Display::pressEnterToContinue();
                break;
            case 5:
            {
                // C++11: Lambda as callback - demonstrates function wrappers
                std::string label = Display::getInput("Callback label (e.g., 'Logger')");

                reminder_.addCallback(
                    [label](const Routine& r)
                    {
                        std::cout << Color::DIM << "  [" << label << "] Reminder for: "
                                  << r.name << Color::RESET << "\n";
                    });

                Display::printSuccess("Callback '" + label + "' registered.");
                Display::pressEnterToContinue();
                break;
            }
            case 0: return;
            default: Display::printError("Invalid choice.");
        }
    }
}

// ============================================================================
// Statistics & Reports sub-menu
// ============================================================================
void Menu::statisticsReports()
{
    while(true)
    {
        Display::clearScreen();
        Display::printHeader("STATISTICS & REPORTS");

        std::cout << "\n";
        Display::printMenuOption(1, "Total Routines");
        Display::printMenuOption(2, "Completed Routines");
        Display::printMenuOption(3, "Pending Routines");
        Display::printMenuOption(4, "Completion Progress");
        Display::printMenuOption(5, "Category-wise Report");
        Display::printMenuOption(6, "Priority-wise Report");
        Display::printMenuOption(0, "Back");

        int choice = Display::getChoice();
        switch(choice)
        {
            case 1:
                Display::printInfo("Total routines: " +
                    std::to_string(stats_.totalRoutines()));
                Display::pressEnterToContinue();
                break;
            case 2:
            {
                Display::printInfo("Completed: " +
                    std::to_string(stats_.completedRoutines()));
                auto names = stats_.completedRoutineNames();
                if(!names.empty())
                {
                    for(const auto& n : names)
                        std::cout << "    " << Color::GREEN << "- " << n << Color::RESET << "\n";
                }
                Display::pressEnterToContinue();
                break;
            }
            case 3:
            {
                Display::printInfo("Pending: " +
                    std::to_string(stats_.pendingRoutines()));
                auto names = stats_.pendingRoutineNames();
                if(!names.empty())
                {
                    for(const auto& n : names)
                        std::cout << "    " << Color::YELLOW << "- " << n << Color::RESET << "\n";
                }
                Display::pressEnterToContinue();
                break;
            }
            case 4:
            {
                Display::printSubHeader("Completion Progress");
                std::cout << "  Total activities committed: " << stats_.totalRoutines() << "\n";
                std::cout << "  Activities completed:       " << Color::GREEN
                          << stats_.completedRoutines() << Color::RESET << "\n";
                std::cout << "  Activities remaining:       " << Color::YELLOW
                          << stats_.pendingRoutines() << Color::RESET << "\n\n";
                Display::printProgressBar(stats_.completionPercentage());

                // Show completed activity names
                auto completed = stats_.completedRoutineNames();
                if(!completed.empty())
                {
                    std::cout << "\n  " << Color::GREEN << "Completed:" << Color::RESET << "\n";
                    for(const auto& n : completed)
                        std::cout << "    " << Color::GREEN << "- " << n << Color::RESET << "\n";
                }

                // Show remaining activity names
                auto pending = stats_.pendingRoutineNames();
                if(!pending.empty())
                {
                    std::cout << "\n  " << Color::YELLOW << "Remaining:" << Color::RESET << "\n";
                    for(const auto& n : pending)
                        std::cout << "    " << Color::YELLOW << "- " << n << Color::RESET << "\n";
                }

                // Appreciation when everything is done!
                if(stats_.completionPercentage() >= 100.0
                   && stats_.totalRoutines() > 0)
                {
                    std::cout << "\n  " << Color::GREEN << Color::BOLD
                              << "*** ALL ROUTINES COMPLETE! ***"
                              << Color::RESET << "\n";
                    std::cout << "  " << Quotes::getAppreciationMessage() << "\n";
                }
                Display::pressEnterToContinue();
                break;
            }
            case 5:
            {
                Display::printSubHeader("Category-wise Report");

                // C++17: Structured bindings unpack the map pair
                auto report = stats_.categoryDetailedReport();
                for(const auto& [category, names] : report)
                {
                    std::cout << "\n  " << Color::CYAN << Color::BOLD
                              << category << Color::RESET
                              << " (" << names.size() << " routine(s)):\n";
                    for(const auto& name : names)
                    {
                        std::cout << "    - " << name << "\n";
                    }
                }
                std::cout << "\n";
                Display::pressEnterToContinue();
                break;
            }
            case 6:
            {
                Display::printSubHeader("Priority-wise Report");

                auto report = stats_.priorityDetailedReport();
                // C++17: Structured bindings
                for(const auto& [priority, names] : report)
                {
                    std::cout << "\n  " << Color::CYAN << Color::BOLD
                              << priority << Color::RESET
                              << " (" << names.size() << " routine(s)):\n";
                    for(const auto& name : names)
                    {
                        std::cout << "    - " << name << "\n";
                    }
                }
                std::cout << "\n";
                Display::pressEnterToContinue();
                break;
            }
            case 0: return;
            default: Display::printError("Invalid choice.");
        }
    }
}

// ============================================================================
// Data Management sub-menu
// ============================================================================
void Menu::dataManagement()
{
    while(true)
    {
        Display::clearScreen();
        Display::printHeader("DATA MANAGEMENT");

        Display::printMenuOption(1, "Save Data");
        Display::printMenuOption(2, "Load Data");
        Display::printMenuOption(3, "Clear All Data");
        Display::printMenuOption(0, "Back");

        int choice = Display::getChoice();
        switch(choice)
        {
            case 1:
                if(data_mgr_.saveData(manager_))
                    Display::printSuccess("Data saved successfully to JSON file.");
                else
                    Display::printError("Failed to save data.");
                Display::pressEnterToContinue();
                break;
            case 2:
                if(data_mgr_.loadData(manager_))
                    Display::printSuccess("Data loaded successfully.");
                else
                    Display::printError("No saved data found or error loading.");
                Display::pressEnterToContinue();
                break;
            case 3:
            {
                std::string confirm = Display::getInput(
                    "Are you sure? Type 'yes' to confirm");
                if(confirm == "yes" || confirm == "y")
                {
                    manager_.clearAll();
                    Display::printSuccess("All data cleared.");
                }
                else
                {
                    Display::printInfo("Cancelled.");
                }
                Display::pressEnterToContinue();
                break;
            }
            case 0: return;
            default: Display::printError("Invalid choice.");
        }
    }
}

// ============================================================================
// ROUTINE SUB-ACTIONS
// ============================================================================

void Menu::addNewRoutine()
{
    Display::printSubHeader("Add New Routine");

    std::string name = Display::getInput("Routine name");
    if(name.empty())
    {
        Display::printError("Name cannot be empty.");
        Display::pressEnterToContinue();
        return;
    }

    std::string desc     = Display::getInput("Description (optional)");
    std::string category = Display::getInput("Category (e.g., Health, Work, Study)");

    std::cout << "\n  Priority levels:\n";
    std::cout << "    1. Low   2. Medium   3. High   4. Critical\n";
    int pri = Display::getIntInput("Priority", 1, 4);

    std::cout << "\n  " << Color::DIM
              << "(Note: In this demo, frequency is in seconds for quick testing."
              << " In production, this would be hours.)" << Color::RESET << "\n";
    int freq = Display::getIntInput("Reminder check interval in seconds for demo (1-24)", 1, 24);

    // C++11: std::move transfers strings efficiently into the manager
    int id = manager_.addRoutine(
        std::move(name), std::move(desc),
        std::move(category), static_cast<Priority>(pri), freq);

    Display::printSuccess("Routine #" + std::to_string(id) + " created!");
    Display::pressEnterToContinue();
}

void Menu::viewAllRoutines()
{
    Display::printSubHeader("All Routines");
    Display::printRoutineList(manager_.getAllRoutines());
    Display::pressEnterToContinue();
}

void Menu::viewPendingRoutines()
{
    Display::printSubHeader("Pending Routines");
    Display::printRoutineList(manager_.getPendingRoutines());
    Display::pressEnterToContinue();
}

void Menu::viewCompletedRoutines()
{
    Display::printSubHeader("Completed Routines");
    Display::printRoutineList(manager_.getCompletedRoutines());
    Display::pressEnterToContinue();
}

void Menu::markRoutineCompleted()
{
    Display::printSubHeader("Mark as Completed");

    auto pending = manager_.getPendingRoutines();
    if(pending.empty())
    {
        Display::printInfo("No pending routines.");
        Display::pressEnterToContinue();
        return;
    }

    Display::printRoutineList(pending);

    int id = Display::getIntInput("Enter Routine ID to complete", 1, 99999);

    // C++17: std::optional - safely check if the routine exists
    auto routine = manager_.getRoutineById(id);
    if(!routine.has_value())
    {
        Display::printError("Routine not found.");
        Display::pressEnterToContinue();
        return;
    }

    if(manager_.markCompleted(id))
    {
        Display::printSuccess("Routine #" + std::to_string(id) + " marked as completed!");

        // Appreciation message (makes user feel good!)
        std::cout << "\n  " << Color::GREEN
                  << Quotes::getAppreciationMessage()
                  << Color::RESET << "\n";

        // Show real-world benefit of the category
        if(!routine.value().category.empty())
        {
            std::cout << "  " << Color::CYAN
                      << Quotes::getBenefitMessage(routine.value().category)
                      << Color::RESET << "\n";
        }

        // Check if ALL routines are now complete
        if(manager_.pendingCount() == 0 && manager_.totalCount() > 0)
        {
            std::cout << "\n  " << Color::GREEN << Color::BOLD
                      << "*** ALL ROUTINES COMPLETE! YOU'RE A STAR! ***"
                      << Color::RESET << "\n";
        }
    }
    else
    {
        Display::printError("Failed to mark routine.");
    }
    Display::pressEnterToContinue();
}

void Menu::updateRoutinePriority()
{
    Display::printSubHeader("Update Priority");
    Display::printRoutineList(manager_.getAllRoutines());

    int id = Display::getIntInput("Enter Routine ID", 1, 99999);
    std::cout << "    1. Low   2. Medium   3. High   4. Critical\n";
    int pri = Display::getIntInput("New priority", 1, 4);

    if(manager_.updatePriority(id, static_cast<Priority>(pri)))
        Display::printSuccess("Priority updated!");
    else
        Display::printError("Routine not found.");
    Display::pressEnterToContinue();
}

void Menu::categorizeRoutine()
{
    Display::printSubHeader("Categorize Routine");
    Display::printRoutineList(manager_.getAllRoutines());

    int id = Display::getIntInput("Enter Routine ID", 1, 99999);
    std::string category = Display::getInput("New category");

    if(manager_.categorizeRoutine(id, category))
        Display::printSuccess("Category updated!");
    else
        Display::printError("Routine not found.");
    Display::pressEnterToContinue();
}

void Menu::deleteRoutine()
{
    Display::printSubHeader("Delete Routine");
    Display::printRoutineList(manager_.getAllRoutines());

    int id = Display::getIntInput("Enter Routine ID to delete", 1, 99999);
    std::string confirm = Display::getInput("Are you sure? (yes/no)");

    if(confirm == "yes" || confirm == "y")
    {
        if(manager_.deleteRoutine(id))
            Display::printSuccess("Routine deleted.");
        else
            Display::printError("Routine not found.");
    }
    else
    {
        Display::printInfo("Cancelled.");
    }
    Display::pressEnterToContinue();
}

void Menu::searchRoutine()
{
    Display::printSubHeader("Search Routines");
    std::string keyword = Display::getInput("Search keyword");

    if(keyword.empty())
    {
        Display::printError("Please enter a keyword.");
        Display::pressEnterToContinue();
        return;
    }

    auto results = manager_.searchRoutines(keyword);
    Display::printInfo("Found " + std::to_string(results.size()) + " result(s):");
    Display::printRoutineList(results);
    Display::pressEnterToContinue();
}
