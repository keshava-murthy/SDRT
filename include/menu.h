#pragma once

#include "routine_manager.h"
#include "reminder_service.h"
#include "statistics.h"
#include "data_manager.h"

// ============================================================================
//   Menu: drives the entire user interface
//   Delegates to RoutineManager, ReminderService, Statistics, DataManager
// ============================================================================
class Menu
{
private:
    RoutineManager&  manager_;
    ReminderService& reminder_;
    Statistics&      stats_;
    DataManager&     data_mgr_;

    // Top-level menus
    void mainMenu();
    void routineManagement();
    void reminderManagement();
    void statisticsReports();
    void dataManagement();

    // Routine sub-actions
    void addNewRoutine();
    void viewAllRoutines();
    void viewPendingRoutines();
    void viewCompletedRoutines();
    void markRoutineCompleted();
    void updateRoutinePriority();
    void categorizeRoutine();
    void deleteRoutine();
    void searchRoutine();

public:
    Menu(RoutineManager& mgr, ReminderService& rem,
         Statistics& st, DataManager& dm);
    void run();
};
