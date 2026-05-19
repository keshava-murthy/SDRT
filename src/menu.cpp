#include "menu.h"
#include "display.h"
#include "quotes.h"
#include <iostream>
#include <iomanip>
#include <string>

Navigator::Navigator(TaskOrchestrator& orch, InsightEngine& ins, VaultKeeper& vk)
    : orchestrator_(orch)
    , insights_(ins)
    , vault_(vk)
{
}

void Navigator::run()
{
    Canvas::wipeScreen();
    Canvas::showBanner("SMART DAILY ROUTINE TRACKER");
    Canvas::showNote("Welcome! Let's make today productive.");

    std::cout << "\n  " << Palette::MAGENTA
              << Spark::pickMotivator()
              << Palette::RESET << "\n";

    if(vault_.restore(orchestrator_))
    {
        Canvas::showOk("Loaded saved routines.");

        size_t pending = orchestrator_.awaitingCount();
        if(pending > 0)
            Canvas::showAlert(std::to_string(pending) + " routine(s) still pending!");
    }

    Canvas::holdForEnter();
    primaryHub();
}

void Navigator::primaryHub()
{
    while(true)
    {
        Canvas::wipeScreen();
        Canvas::showBanner("SMART DAILY ROUTINE TRACKER");

        if(orchestrator_.headcount() > 0)
        {
            Canvas::showBar(insights_.progressRatio());
            std::cout << "  "
                      << Palette::GREEN << insights_.doneCount() << " routine(s) done"
                      << Palette::RESET << " | "
                      << Palette::YELLOW << insights_.leftoverCount() << " routine(s) pending"
                      << Palette::RESET << " | "
                      << insights_.overallTally() << " routine(s) total\n";
        }

        std::cout << "\n";
        Canvas::showOption(1, "Routine Management");
        Canvas::showOption(2, "Statistics & Reports");
        Canvas::showOption(3, "Data Management");
        Canvas::showOption(0, "Exit");

        int choice = Canvas::grabChoice();
        switch(choice)
        {
            case 1: taskControls();  break;
            case 2: insightPanel();  break;
            case 3: vaultOps();      break;
            case 0:
                if(vault_.persist(orchestrator_))
                    Canvas::showOk("Data saved automatically.");
                Canvas::showNote("Goodbye! Stay productive!");
                return;
            default:
                Canvas::showFail("Invalid choice.");
        }
    }
}

void Navigator::taskControls()
{
    while(true)
    {
        Canvas::wipeScreen();
        Canvas::showBanner("ROUTINE MANAGEMENT");

        Canvas::showOption(1, "Add New Routine");
        Canvas::showOption(2, "View All Routines");
        Canvas::showOption(3, "View Pending Routines");
        Canvas::showOption(4, "View Completed Routines");
        Canvas::showOption(5, "Mark Routine as Completed");
        Canvas::showOption(6, "Update Priority");
        Canvas::showOption(7, "Categorize Routine");
        Canvas::showOption(8, "Delete Routine");
        Canvas::showOption(9, "Search Routine");
        Canvas::showOption(0, "Back");

        int choice = Canvas::grabChoice();
        switch(choice)
        {
            case 1: enrollNew();         break;
            case 2: browseAll();         break;
            case 3: browsePending();     break;
            case 4: browseFinished();    break;
            case 5: sealTask();          break;
            case 6: reshuffleUrgency();  break;
            case 7: relabelTask();       break;
            case 8: discardTask();       break;
            case 9: huntTask();          break;
            case 0: return;
            default: Canvas::showFail("Invalid choice.");
        }
    }
}

void Navigator::insightPanel()
{
    while(true)
    {
        Canvas::wipeScreen();
        Canvas::showBanner("STATISTICS & REPORTS");

        std::cout << "\n";
        Canvas::showOption(1, "Total Routines");
        Canvas::showOption(2, "Completed Routines");
        Canvas::showOption(3, "Pending Routines");
        Canvas::showOption(4, "Completion Progress");
        Canvas::showOption(5, "Category-wise Report");
        Canvas::showOption(6, "Priority-wise Report");
        Canvas::showOption(0, "Back");

        int choice = Canvas::grabChoice();
        switch(choice)
        {
            case 1:
                Canvas::showNote("Total routines: " +
                    std::to_string(insights_.overallTally()));
                Canvas::holdForEnter();
                break;
            case 2:
            {
                Canvas::showNote("Completed: " +
                    std::to_string(insights_.doneCount()));
                auto names = insights_.doneNames();
                if(!names.empty())
                {
                    for(const auto& n : names)
                        std::cout << "    " << Palette::GREEN << "- " << n << Palette::RESET << "\n";
                }
                Canvas::holdForEnter();
                break;
            }
            case 3:
            {
                Canvas::showNote("Pending: " +
                    std::to_string(insights_.leftoverCount()));
                auto names = insights_.leftoverNames();
                if(!names.empty())
                {
                    for(const auto& n : names)
                        std::cout << "    " << Palette::YELLOW << "- " << n << Palette::RESET << "\n";
                }
                Canvas::holdForEnter();
                break;
            }
            case 4:
            {
                Canvas::showCaption("Completion Progress");
                std::cout << "  Total activities committed: " << insights_.overallTally() << "\n";
                std::cout << "  Activities completed:       " << Palette::GREEN
                          << insights_.doneCount() << Palette::RESET << "\n";
                std::cout << "  Activities remaining:       " << Palette::YELLOW
                          << insights_.leftoverCount() << Palette::RESET << "\n\n";
                Canvas::showBar(insights_.progressRatio());

                auto completed = insights_.doneNames();
                if(!completed.empty())
                {
                    std::cout << "\n  " << Palette::GREEN << "Completed:" << Palette::RESET << "\n";
                    for(const auto& n : completed)
                        std::cout << "    " << Palette::GREEN << "- " << n << Palette::RESET << "\n";
                }

                auto pending = insights_.leftoverNames();
                if(!pending.empty())
                {
                    std::cout << "\n  " << Palette::YELLOW << "Remaining:" << Palette::RESET << "\n";
                    for(const auto& n : pending)
                        std::cout << "    " << Palette::YELLOW << "- " << n << Palette::RESET << "\n";
                }

                if(insights_.progressRatio() >= 100.0
                   && insights_.overallTally() > 0)
                {
                    std::cout << "\n  " << Palette::GREEN << Palette::BOLD
                              << "*** ALL ROUTINES COMPLETE! ***"
                              << Palette::RESET << "\n";
                    std::cout << "  " << Spark::pickPraise() << "\n";
                }
                Canvas::holdForEnter();
                break;
            }
            case 5:
            {
                Canvas::showCaption("Category-wise Report");

                auto report = insights_.labelBreakdown();
                for(const auto& [category, names] : report)
                {
                    std::cout << "\n  " << Palette::CYAN << Palette::BOLD
                              << category << Palette::RESET
                              << " (" << names.size() << " routine(s)):\n";
                    for(const auto& name : names)
                        std::cout << "    - " << name << "\n";
                }
                std::cout << "\n";
                Canvas::holdForEnter();
                break;
            }
            case 6:
            {
                Canvas::showCaption("Priority-wise Report");

                auto report = insights_.urgencyBreakdown();
                for(const auto& [priority, names] : report)
                {
                    std::cout << "\n  " << Palette::CYAN << Palette::BOLD
                              << priority << Palette::RESET
                              << " (" << names.size() << " routine(s)):\n";
                    for(const auto& name : names)
                        std::cout << "    - " << name << "\n";
                }
                std::cout << "\n";
                Canvas::holdForEnter();
                break;
            }
            case 0: return;
            default: Canvas::showFail("Invalid choice.");
        }
    }
}

void Navigator::vaultOps()
{
    while(true)
    {
        Canvas::wipeScreen();
        Canvas::showBanner("DATA MANAGEMENT");

        Canvas::showOption(1, "Save Data");
        Canvas::showOption(2, "Load Data");
        Canvas::showOption(3, "Clear All Data");
        Canvas::showOption(0, "Back");

        int choice = Canvas::grabChoice();
        switch(choice)
        {
            case 1:
                if(vault_.persist(orchestrator_))
                    Canvas::showOk("Data saved successfully to JSON file.");
                else
                    Canvas::showFail("Failed to save data.");
                Canvas::holdForEnter();
                break;
            case 2:
                if(vault_.restore(orchestrator_))
                    Canvas::showOk("Data loaded successfully.");
                else
                    Canvas::showFail("No saved data found or error loading.");
                Canvas::holdForEnter();
                break;
            case 3:
            {
                std::string confirm = Canvas::grabInput(
                    "Are you sure? Type 'yes' to confirm");
                if(confirm == "yes" || confirm == "y")
                {
                    orchestrator_.wipeSlate();
                    Canvas::showOk("All data cleared.");
                }
                else
                {
                    Canvas::showNote("Cancelled.");
                }
                Canvas::holdForEnter();
                break;
            }
            case 0: return;
            default: Canvas::showFail("Invalid choice.");
        }
    }
}

void Navigator::enrollNew()
{
    Canvas::showCaption("Add New Routine");

    std::string name = Canvas::grabInput("Routine name");
    if(name.empty())
    {
        Canvas::showFail("Name cannot be empty.");
        Canvas::holdForEnter();
        return;
    }

    std::string desc     = Canvas::grabInput("Description (optional)");
    std::string category = Canvas::grabInput("Category (e.g., Health, Work, Study)");

    std::cout << "\n  Priority levels:\n";
    std::cout << "    1. Low   2. Medium   3. High   4. Critical\n";
    int pri = Canvas::grabInt("Priority", 1, 4);

    int freq = Canvas::grabInt("Reminder frequency in hours (1-24)", 1, 24);

    int id = orchestrator_.enrollTask(
        std::move(name), std::move(desc),
        std::move(category), static_cast<Urgency>(pri), freq);

    Canvas::showOk("Routine #" + std::to_string(id) + " created!");
    Canvas::holdForEnter();
}

void Navigator::browseAll()
{
    Canvas::showCaption("All Routines");
    Canvas::showEntryList(orchestrator_.fetchAll());
    Canvas::holdForEnter();
}

void Navigator::browsePending()
{
    Canvas::showCaption("Pending Routines");
    Canvas::showEntryList(orchestrator_.fetchPending());
    Canvas::holdForEnter();
}

void Navigator::browseFinished()
{
    Canvas::showCaption("Completed Routines");
    Canvas::showEntryList(orchestrator_.fetchFinished());
    Canvas::holdForEnter();
}

void Navigator::sealTask()
{
    Canvas::showCaption("Mark as Completed");

    auto pending = orchestrator_.fetchPending();
    if(pending.empty())
    {
        Canvas::showNote("No pending routines.");
        Canvas::holdForEnter();
        return;
    }

    Canvas::showEntryList(pending);

    int id = Canvas::grabInt("Enter Routine ID to complete", 1, 99999);

    auto entry = orchestrator_.pluckById(id);
    if(!entry.has_value())
    {
        Canvas::showFail("Routine not found.");
        Canvas::holdForEnter();
        return;
    }

    if(orchestrator_.sealAsDone(id))
    {
        Canvas::showOk("Routine #" + std::to_string(id) + " marked as completed!");

        std::cout << "\n  " << Palette::GREEN
                  << Spark::pickPraise()
                  << Palette::RESET << "\n";

        if(!entry.value().category.empty())
        {
            std::cout << "  " << Palette::CYAN
                      << Spark::pickPerks(entry.value().category)
                      << Palette::RESET << "\n";
        }

        if(orchestrator_.awaitingCount() == 0 && orchestrator_.headcount() > 0)
        {
            std::cout << "\n  " << Palette::GREEN << Palette::BOLD
                      << "*** ALL ROUTINES COMPLETE! YOU'RE A STAR! ***"
                      << Palette::RESET << "\n";
        }
    }
    else
    {
        Canvas::showFail("Failed to mark routine.");
    }
    Canvas::holdForEnter();
}

void Navigator::reshuffleUrgency()
{
    Canvas::showCaption("Update Priority");
    Canvas::showEntryList(orchestrator_.fetchAll());

    int id = Canvas::grabInt("Enter Routine ID", 1, 99999);
    std::cout << "    1. Low   2. Medium   3. High   4. Critical\n";
    int pri = Canvas::grabInt("New priority", 1, 4);

    if(orchestrator_.shiftUrgency(id, static_cast<Urgency>(pri)))
        Canvas::showOk("Priority updated!");
    else
        Canvas::showFail("Routine not found.");
    Canvas::holdForEnter();
}

void Navigator::relabelTask()
{
    Canvas::showCaption("Categorize Routine");
    Canvas::showEntryList(orchestrator_.fetchAll());

    int id = Canvas::grabInt("Enter Routine ID", 1, 99999);
    std::string category = Canvas::grabInput("New category");

    if(orchestrator_.relabelTask(id, category))
        Canvas::showOk("Category updated!");
    else
        Canvas::showFail("Routine not found.");
    Canvas::holdForEnter();
}

void Navigator::discardTask()
{
    Canvas::showCaption("Delete Routine");
    Canvas::showEntryList(orchestrator_.fetchAll());

    int id = Canvas::grabInt("Enter Routine ID to delete", 1, 99999);
    std::string confirm = Canvas::grabInput("Are you sure? (yes/no)");

    if(confirm == "yes" || confirm == "y")
    {
        if(orchestrator_.discardTask(id))
            Canvas::showOk("Routine deleted.");
        else
            Canvas::showFail("Routine not found.");
    }
    else
    {
        Canvas::showNote("Cancelled.");
    }
    Canvas::holdForEnter();
}

void Navigator::huntTask()
{
    Canvas::showCaption("Search Routines");
    std::string keyword = Canvas::grabInput("Search keyword");

    if(keyword.empty())
    {
        Canvas::showFail("Please enter a keyword.");
        Canvas::holdForEnter();
        return;
    }

    auto results = orchestrator_.huntByKeyword(keyword);
    Canvas::showNote("Found " + std::to_string(results.size()) + " result(s):");
    Canvas::showEntryList(results);
    Canvas::holdForEnter();
}
