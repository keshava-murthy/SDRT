#include "routine_manager.h"
#include <algorithm>

TaskOrchestrator::TaskOrchestrator()
    : next_id_(1)
{
}

void TaskOrchestrator::refreshLookup()
{
    lookup_.clear();
    for(size_t i = 0; i < entries_.size(); ++i)
    {
        lookup_[entries_[i].id] = i;
    }
}

int TaskOrchestrator::enrollTask(std::string name, std::string desc,
                                  std::string category, Urgency urgency,
                                  int freq_hours)
{
    int id = next_id_++;
    entries_.emplace_back(id, std::move(name), std::move(desc),
                          std::move(category), urgency, freq_hours);
    lookup_[id] = entries_.size() - 1;
    return id;
}

bool TaskOrchestrator::discardTask(int id)
{
    auto it = lookup_.find(id);
    if(it == lookup_.end()) return false;

    entries_.erase(entries_.begin() + static_cast<long>(it->second));
    refreshLookup();
    return true;
}

bool TaskOrchestrator::sealAsDone(int id)
{
    auto it = lookup_.find(id);
    if(it == lookup_.end()) return false;

    entries_[it->second].finished  = true;
    entries_[it->second].sealed_at = TaskEntry::stampNow();
    return true;
}

bool TaskOrchestrator::shiftUrgency(int id, Urgency new_urgency)
{
    auto it = lookup_.find(id);
    if(it == lookup_.end()) return false;

    entries_[it->second].urgency = new_urgency;
    return true;
}

bool TaskOrchestrator::relabelTask(int id, const std::string& category)
{
    auto it = lookup_.find(id);
    if(it == lookup_.end()) return false;

    entries_[it->second].category = category;
    return true;
}

std::vector<TaskEntry> TaskOrchestrator::fetchAll() const
{
    return entries_;
}

std::vector<TaskEntry> TaskOrchestrator::siftTasks(
    std::function<bool(const TaskEntry&)> predicate) const
{
    std::vector<TaskEntry> result;
    for(const auto& e : entries_)
    {
        if(predicate(e))
            result.push_back(e);
    }
    return result;
}

std::vector<TaskEntry> TaskOrchestrator::fetchPending() const
{
    return siftTasks([](const TaskEntry& e) { return !e.finished; });
}

std::vector<TaskEntry> TaskOrchestrator::fetchFinished() const
{
    return siftTasks([](const TaskEntry& e) { return e.finished; });
}

std::vector<TaskEntry> TaskOrchestrator::huntByKeyword(const std::string& keyword) const
{
    std::string lower_kw = keyword;
    std::transform(lower_kw.begin(), lower_kw.end(), lower_kw.begin(), ::tolower);

    return siftTasks([&lower_kw](const TaskEntry& e)
    {
        auto toLower = [](const std::string& s)
        {
            std::string out = s;
            std::transform(out.begin(), out.end(), out.begin(), ::tolower);
            return out;
        };

        return toLower(e.name).find(lower_kw)        != std::string::npos
            || toLower(e.description).find(lower_kw) != std::string::npos
            || toLower(e.category).find(lower_kw)    != std::string::npos;
    });
}

std::optional<TaskEntry> TaskOrchestrator::pluckById(int id) const
{
    auto it = lookup_.find(id);
    if(it == lookup_.end())
        return std::nullopt;
    return entries_[it->second];
}

size_t TaskOrchestrator::headcount() const
{
    return entries_.size();
}

size_t TaskOrchestrator::finishedCount() const
{
    return static_cast<size_t>(
        std::count_if(entries_.begin(), entries_.end(),
                      [](const TaskEntry& e) { return e.finished; }));
}

size_t TaskOrchestrator::awaitingCount() const
{
    return static_cast<size_t>(
        std::count_if(entries_.begin(), entries_.end(),
                      [](const TaskEntry& e) { return !e.finished; }));
}

const std::vector<TaskEntry>& TaskOrchestrator::peekAll() const
{
    return entries_;
}

void TaskOrchestrator::ingestTasks(std::vector<TaskEntry> fresh)
{
    entries_ = std::move(fresh);
    refreshLookup();

    next_id_ = 1;
    for(const auto& e : entries_)
    {
        if(e.id >= next_id_)
            next_id_ = e.id + 1;
    }
}

void TaskOrchestrator::wipeSlate()
{
    entries_.clear();
    lookup_.clear();
    next_id_ = 1;
}
