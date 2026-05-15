#pragma once

#include "routine_manager.h"
#include <string>

// ============================================================================
// DataManager: saves/loads routines to/from a JSON file
//   Uses nlohmann/json (json.hpp) for serialization
//   Uses C++17 std::filesystem to create directories automatically
// ============================================================================
class DataManager
{
private:
    std::string filepath_;

public:
    // C++11: explicit prevents accidental implicit conversions
    explicit DataManager(std::string path = "data/routines.json");

    bool saveData(const RoutineManager& manager);
    bool loadData(RoutineManager& manager);
};
