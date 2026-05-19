#pragma once

#include "routine.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <optional>

class TaskOrchestrator
{
private:
    std::vector<TaskEntry> entries_;
    std::unordered_map<int, size_t> lookup_;
    int next_id_;

    void refreshLookup();

public:
    TaskOrchestrator();

    int  enrollTask(std::string name, std::string desc,
                    std::string category, Urgency urgency, int freq_hours = 24);
    bool discardTask(int id);
    bool sealAsDone(int id);
    bool shiftUrgency(int id, Urgency new_urgency);
    bool relabelTask(int id, const std::string& category);

    std::vector<TaskEntry> fetchAll() const;
    std::vector<TaskEntry> siftTasks(
        std::function<bool(const TaskEntry&)> predicate) const;
    std::vector<TaskEntry> fetchPending() const;
    std::vector<TaskEntry> fetchFinished() const;
    std::vector<TaskEntry> huntByKeyword(const std::string& keyword) const;

    std::optional<TaskEntry> pluckById(int id) const;

    size_t headcount() const;
    size_t finishedCount() const;
    size_t awaitingCount() const;

    const std::vector<TaskEntry>& peekAll() const;
    void ingestTasks(std::vector<TaskEntry> fresh);
    void wipeSlate();
};
