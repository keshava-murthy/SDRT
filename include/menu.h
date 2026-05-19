#pragma once

#include "routine_manager.h"
#include "statistics.h"
#include "data_manager.h"

// ============================================================================
//   Menu: drives the entire user interface
//   Delegates to RoutineManager, Statistics, and DataManager
//   (Reminder service will be added in a future update with threading support)
// ============================================================================
class Menu
{
private:
    RoutineManager&  manager_;
    Statistics&      stats_;
    DataManager&     data_mgr_;

    // Top-level menus
    void mainMenu();
    void routineManagement();
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
    Menu(RoutineManager& mgr, Statistics& st, DataManager& dm);
    void run();
};
