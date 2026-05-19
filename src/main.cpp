#include "routine_manager.h"
#include "statistics.h"
#include "data_manager.h"
#include "menu.h"

#include <memory>
#include <iostream>

int main()
{
    auto orchestrator = std::make_unique<TaskOrchestrator>();
    auto insights     = std::make_unique<InsightEngine>(*orchestrator);
    auto vault        = std::make_unique<VaultKeeper>("data/routines.json");

    Navigator nav(*orchestrator, *insights, *vault);
    nav.run();

    return 0;
}
