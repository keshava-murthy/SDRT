#include "data_manager.h"
#include "display.h"
#include "json.h"
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

VaultKeeper::VaultKeeper(std::string path)
    : filepath_(std::move(path))
{
}

bool VaultKeeper::persist(const TaskOrchestrator& orchestrator)
{
    std::filesystem::path dir = std::filesystem::path(filepath_).parent_path();
    if(!dir.empty() && !std::filesystem::exists(dir))
        std::filesystem::create_directories(dir);

    json j;
    j["routines"] = json::array();

    for(const auto& e : orchestrator.peekAll())
    {
        json ej;
        ej["id"]                      = e.id;
        ej["name"]                    = e.name;
        ej["description"]             = e.description;
        ej["category"]                = e.category;
        ej["priority"]                = static_cast<int>(e.urgency);
        ej["completed"]               = e.finished;
        ej["created_at"]              = e.born_at;
        ej["completed_at"]            = e.sealed_at;
        ej["notification_freq_hours"] = e.nudge_interval_hrs;

        if(e.alert_moment.has_value())
            ej["reminder_time"] = e.alert_moment.value();
        else
            ej["reminder_time"] = nullptr;

        j["routines"].push_back(ej);
    }

    std::ofstream file(filepath_);
    if(!file.is_open()) return false;

    file << j.dump(4);
    return true;
}

bool VaultKeeper::restore(TaskOrchestrator& orchestrator)
{
    if(!std::filesystem::exists(filepath_))
        return false;

    std::ifstream file(filepath_);
    if(!file.is_open()) return false;

    try
    {
        json j;
        file >> j;

        std::vector<TaskEntry> entries;

        for(const auto& item : j["routines"])
        {
            TaskEntry e;
            e.id                 = item["id"].get<int>();
            e.name               = item["name"].get<std::string>();
            e.description        = item["description"].get<std::string>();
            e.category           = item["category"].get<std::string>();
            e.urgency            = static_cast<Urgency>(item["priority"].get<int>());
            e.finished           = item["completed"].get<bool>();
            e.born_at            = item["created_at"].get<std::string>();
            e.sealed_at          = item["completed_at"].get<std::string>();
            e.nudge_interval_hrs = item["notification_freq_hours"].get<int>();

            if(item.contains("reminder_time") && !item["reminder_time"].is_null())
                e.alert_moment = item["reminder_time"].get<std::string>();

            entries.push_back(std::move(e));
        }

        orchestrator.ingestTasks(std::move(entries));
        return true;
    }
    catch(const std::exception&)
    {
        return false;
    }
}
