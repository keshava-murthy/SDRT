#pragma once

#include "routine_manager.h"
#include <map>
#include <vector>
#include <string>

class InsightEngine
{
private:
    const TaskOrchestrator& orchestrator_;

public:
    explicit InsightEngine(const TaskOrchestrator& orch);

    size_t overallTally() const;
    size_t doneCount() const;
    size_t leftoverCount() const;
    double progressRatio() const;

    std::map<std::string, int> groupByLabel() const;
    std::map<std::string, int> groupByUrgency() const;

    std::map<std::string, std::vector<std::string>> labelBreakdown() const;
    std::map<std::string, std::vector<std::string>> urgencyBreakdown() const;

    std::vector<std::string> doneNames() const;
    std::vector<std::string> leftoverNames() const;
};
