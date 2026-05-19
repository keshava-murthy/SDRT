#pragma once

#include "routine_manager.h"
#include <string>

class VaultKeeper
{
private:
    std::string filepath_;

public:
    explicit VaultKeeper(std::string path = "data/routines.json");

    bool persist(const TaskOrchestrator& orchestrator);
    bool restore(TaskOrchestrator& orchestrator);
};
