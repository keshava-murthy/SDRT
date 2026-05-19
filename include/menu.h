#pragma once

#include "routine_manager.h"
#include "statistics.h"
#include "data_manager.h"

class Navigator
{
private:
    TaskOrchestrator& orchestrator_;
    InsightEngine&    insights_;
    VaultKeeper&      vault_;

    void primaryHub();
    void taskControls();
    void insightPanel();
    void vaultOps();

    void enrollNew();
    void browseAll();
    void browsePending();
    void browseFinished();
    void sealTask();
    void reshuffleUrgency();
    void relabelTask();
    void discardTask();
    void huntTask();

public:
    Navigator(TaskOrchestrator& orch, InsightEngine& ins, VaultKeeper& vk);
    void run();
};
