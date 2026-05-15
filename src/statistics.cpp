#include "statistics.h"

Statistics::Statistics(const RoutineManager& mgr)
    : manager_(mgr)
{
}

size_t Statistics::totalRoutines() const
{
    return manager_.totalCount();
}

size_t Statistics::completedRoutines() const
{
    return manager_.completedCount();
}

size_t Statistics::pendingRoutines() const
{
    return manager_.pendingCount();
}

double Statistics::completionPercentage() const
{
    size_t total = manager_.totalCount();
    if(total == 0) return 0.0;

    return (static_cast<double>(manager_.completedCount()) / total) * 100.0;
}

// ============================================================================
// Category-wise report
//   C++11: Range-based for with const auto&
//   C++11: std::map automatically sorts categories alphabetically
// ============================================================================
std::map<std::string, int> Statistics::categoryReport() const
{
    std::map<std::string, int> report;

    for(const auto& r : manager_.getAllRoutines())
    {
        std::string cat = r.category.empty() ? "Uncategorized" : r.category;
        report[cat]++;
    }
    return report;
}

// ============================================================================
// Priority-wise report
// ============================================================================
std::map<std::string, int> Statistics::priorityReport() const
{
    std::map<std::string, int> report;

    for(const auto& r : manager_.getAllRoutines())
    {
        report[Routine::priorityToString(r.priority)]++;
    }
    return report;
}
