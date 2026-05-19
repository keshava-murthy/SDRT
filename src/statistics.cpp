#include "statistics.h"

InsightEngine::InsightEngine(const TaskOrchestrator& orch)
    : orchestrator_(orch)
{
}

size_t InsightEngine::overallTally() const
{
    return orchestrator_.headcount();
}

size_t InsightEngine::doneCount() const
{
    return orchestrator_.finishedCount();
}

size_t InsightEngine::leftoverCount() const
{
    return orchestrator_.awaitingCount();
}

double InsightEngine::progressRatio() const
{
    size_t total = orchestrator_.headcount();
    if(total == 0) return 0.0;
    return (static_cast<double>(orchestrator_.finishedCount()) / total) * 100.0;
}

std::map<std::string, int> InsightEngine::groupByLabel() const
{
    std::map<std::string, int> report;
    for(const auto& e : orchestrator_.fetchAll())
    {
        std::string cat = e.category.empty() ? "Uncategorized" : e.category;
        report[cat]++;
    }
    return report;
}

std::map<std::string, int> InsightEngine::groupByUrgency() const
{
    std::map<std::string, int> report;
    for(const auto& e : orchestrator_.fetchAll())
    {
        report[TaskEntry::urgencyLabel(e.urgency)]++;
    }
    return report;
}

std::map<std::string, std::vector<std::string>> InsightEngine::labelBreakdown() const
{
    std::map<std::string, std::vector<std::string>> report;
    for(const auto& e : orchestrator_.fetchAll())
    {
        std::string cat = e.category.empty() ? "Uncategorized" : e.category;
        std::string status = e.finished ? " [DONE]" : " [PENDING]";
        report[cat].push_back(e.name + status);
    }
    return report;
}

std::map<std::string, std::vector<std::string>> InsightEngine::urgencyBreakdown() const
{
    std::map<std::string, std::vector<std::string>> report;
    for(const auto& e : orchestrator_.fetchAll())
    {
        std::string status = e.finished ? " [DONE]" : " [PENDING]";
        report[TaskEntry::urgencyLabel(e.urgency)].push_back(e.name + status);
    }
    return report;
}

std::vector<std::string> InsightEngine::doneNames() const
{
    std::vector<std::string> names;
    for(const auto& e : orchestrator_.fetchAll())
    {
        if(e.finished) names.push_back(e.name);
    }
    return names;
}

std::vector<std::string> InsightEngine::leftoverNames() const
{
    std::vector<std::string> names;
    for(const auto& e : orchestrator_.fetchAll())
    {
        if(!e.finished) names.push_back(e.name);
    }
    return names;
}
