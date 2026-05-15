#include "data_manager.h"
#include "display.h"
#include "json.h"       // nlohmann/json - header-only JSON library
#include <fstream>
#include <filesystem>     // C++17: filesystem operations (create dirs, check existence)

// C++11: 'using' alias - gives nlohmann::json a short name
using json = nlohmann::json;

// ============================================================================
//   Constructor
//   C++11: std::move transfers the string efficiently
// ============================================================================
DataManager::DataManager(std::string path)
    : filepath_(std::move(path))
{
}

// ============================================================================
//   Save all routines to a JSON file
//   Uses nlohmann/json for clean serialization
//   C++17: std::filesystem to auto-create the data/ directory
// ============================================================================
bool DataManager::saveData(const RoutineManager& manager)
{
    // C++17: std::filesystem - create parent directories if they don't exist
    std::filesystem::path dir = std::filesystem::path(filepath_).parent_path();
    if(!dir.empty() && !std::filesystem::exists(dir))
    {
        std::filesystem::create_directories(dir);
    }

    json j;
    j["routines"] = json::array();

    // C++11: Range-based for loop over all routines
    for(const auto& r : manager.getRoutinesRef())
    {
        json rj;
        rj["id"]                      = r.id;
        rj["name"]                    = r.name;
        rj["description"]             = r.description;
        rj["category"]                = r.category;
        rj["priority"]                = static_cast<int>(r.priority);
        rj["completed"]               = r.completed;
        rj["created_at"]              = r.created_at;
        rj["completed_at"]            = r.completed_at;
        rj["notification_freq_hours"] = r.notification_freq_hours;

        // C++17: std::optional - save only if a reminder time is set
        if(r.reminder_time.has_value())
        {
            rj["reminder_time"] = r.reminder_time.value();
        }
        else
        {
            rj["reminder_time"] = nullptr;
        }

        j["routines"].push_back(rj);
    }

    std::ofstream file(filepath_);
    if(!file.is_open()) return false;

    file << j.dump(4);  // pretty-printed with 4-space indent
    return true;
}

// ============================================================================
//   Load routines from a JSON file
// ============================================================================
bool DataManager::loadData(RoutineManager& manager)
{
    // C++17: std::filesystem::exists - clean file existence check
    if(!std::filesystem::exists(filepath_))
    {
        return false;  // no saved data yet, not an error
    }

    std::ifstream file(filepath_);
    if(!file.is_open()) return false;

    try
    {
        json j;
        file >> j;

        std::vector<Routine> routines;

        for(const auto& item : j["routines"])
        {
            Routine r;
            r.id                      = item["id"].get<int>();
            r.name                    = item["name"].get<std::string>();
            r.description             = item["description"].get<std::string>();
            r.category                = item["category"].get<std::string>();
            r.priority                = static_cast<Priority>(item["priority"].get<int>());
            r.completed               = item["completed"].get<bool>();
            r.created_at              = item["created_at"].get<std::string>();
            r.completed_at            = item["completed_at"].get<std::string>();
            r.notification_freq_hours = item["notification_freq_hours"].get<int>();

            // C++17: Load optional reminder_time
            if(item.contains("reminder_time") && !item["reminder_time"].is_null())
            {
                r.reminder_time = item["reminder_time"].get<std::string>();
            }

            // C++11: std::move pushes the routine efficiently
            routines.push_back(std::move(r));
        }

        // C++11: Move the entire vector into the manager
        manager.setRoutines(std::move(routines));
        return true;
    }
    catch(const std::exception&)
    {
        return false;
    }
}
