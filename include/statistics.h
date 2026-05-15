#pragma once

#include "routine_manager.h"
#include <map>      // C++11 STL: ordered map (keys sorted automatically)
#include <vector>
#include <string>

// ============================================================================
//   Statistics: generates reports from the routine data
//   Uses std::map for ordered category/priority breakdowns
// ============================================================================
class Statistics
{
private:
    const RoutineManager& manager_;

public:
    explicit Statistics(const RoutineManager& mgr);

    size_t totalRoutines() const;
    size_t completedRoutines() const;
    size_t pendingRoutines() const;
    double completionPercentage() const;

    // C++11: std::map - sorted associative container
    //   Returns category_name -> count and priority_name -> count
    std::map<std::string, int> categoryReport() const;
    std::map<std::string, int> priorityReport() const;

    // Detailed reports: category/priority -> list of routine names
    std::map<std::string, std::vector<std::string>> categoryDetailedReport() const;
    std::map<std::string, std::vector<std::string>> priorityDetailedReport() const;

    // Get routine names by completion status
    std::vector<std::string> completedRoutineNames() const;
    std::vector<std::string> pendingRoutineNames() const;
};
