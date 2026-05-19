#include "routine_manager.h"
#include <algorithm>  // for std::count_if, std::transform

// ============================================================================
// Constructor: start IDs at 1
// ============================================================================
RoutineManager::RoutineManager()
    : next_id_(1)
{
}

// ============================================================================
// Rebuild the ID -> index mapping after any structural change
// ============================================================================
void RoutineManager::rebuildIndex()
{
    id_index_.clear();
    for(size_t i = 0; i < routines_.size(); ++i)
    {
        id_index_[routines_[i].id] = i;
    }
}

// ============================================================================
// Add a new routine
//   emplace_back constructs the Routine directly in the vector, avoiding
//   unnecessary copies. std::move transfers string ownership efficiently.
// ============================================================================
int RoutineManager::addRoutine(std::string name, std::string desc,
                                std::string category, Priority priority,
                                int freq_hours)
{
    int id = next_id_++;

    // emplace_back constructs the object in-place inside the vector
    routines_.emplace_back(id, std::move(name), std::move(desc),
                           std::move(category), priority, freq_hours);

    id_index_[id] = routines_.size() - 1;
    return id;
}

// ============================================================================
// Delete a routine by ID
// ============================================================================
bool RoutineManager::deleteRoutine(int id)
{
    auto it = id_index_.find(id);
    if(it == id_index_.end()) return false;

    routines_.erase(routines_.begin() + static_cast<long>(it->second));
    rebuildIndex();  // indices shifted after erase, so rebuild the map
    return true;
}

// ============================================================================
// Mark a routine as completed (records completion time)
// ============================================================================
bool RoutineManager::markCompleted(int id)
{
    auto it = id_index_.find(id);
    if(it == id_index_.end()) return false;

    routines_[it->second].completed    = true;
    routines_[it->second].completed_at = Routine::currentTimeStr();
    return true;
}

// ============================================================================
// Change the priority of an existing routine
// ============================================================================
bool RoutineManager::updatePriority(int id, Priority new_priority)
{
    auto it = id_index_.find(id);
    if(it == id_index_.end()) return false;

    routines_[it->second].priority = new_priority;
    return true;
}

// ============================================================================
// Assign or change the category of a routine
// ============================================================================
bool RoutineManager::categorizeRoutine(int id, const std::string& category)
{
    auto it = id_index_.find(id);
    if(it == id_index_.end()) return false;

    routines_[it->second].category = category;
    return true;
}

// ============================================================================
// Get all routines (returns a copy for safety)
// ============================================================================
std::vector<Routine> RoutineManager::getAllRoutines() const
{
    return routines_;
}

// ============================================================================
// Filter routines using a predicate (lambda, functor, or function pointer)
//
//   std::function<bool(const Routine&)> is a "function wrapper" -
//   it can hold any callable that matches the signature:
//     - Lambda:   filterRoutines([](const Routine& r){ return r.completed; })
//     - Function: filterRoutines(isCompleted)
//     - Functor:  filterRoutines(MyChecker{})
// ============================================================================
std::vector<Routine> RoutineManager::filterRoutines(
    std::function<bool(const Routine&)> predicate) const
{
    std::vector<Routine> result;

    for(const auto& r : routines_)
    {
        if(predicate(r))
        {
            result.push_back(r);
        }
    }
    return result;
}

// ============================================================================
// Convenience filters using lambdas
// ============================================================================
std::vector<Routine> RoutineManager::getPendingRoutines() const
{
    return filterRoutines([](const Routine& r) { return !r.completed; });
}

std::vector<Routine> RoutineManager::getCompletedRoutines() const
{
    return filterRoutines([](const Routine& r) { return r.completed; });
}

std::vector<Routine> RoutineManager::searchRoutines(const std::string& keyword) const
{
    // Convert keyword to lowercase for case-insensitive matching
    std::string lower_keyword = keyword;
    std::transform(lower_keyword.begin(), lower_keyword.end(),
                   lower_keyword.begin(), ::tolower);

    // Lambda captures the keyword by reference so we can use it inside
    return filterRoutines([&lower_keyword](const Routine& r)
    {
        auto toLower = [](const std::string& s)
        {
            std::string result = s;
            std::transform(result.begin(), result.end(), result.begin(), ::tolower);
            return result;
        };

        return toLower(r.name).find(lower_keyword)        != std::string::npos
            || toLower(r.description).find(lower_keyword) != std::string::npos
            || toLower(r.category).find(lower_keyword)    != std::string::npos;
    });
}

// ============================================================================
// Get a single routine by ID
//   std::optional - returns empty if routine not found, much cleaner than
//   returning a pointer or throwing an exception for a "not found" case
// ============================================================================
std::optional<Routine> RoutineManager::getRoutineById(int id) const
{
    auto it = id_index_.find(id);
    if(it == id_index_.end())
    {
        return std::nullopt;  // nothing found
    }
    return routines_[it->second];
}

// ============================================================================
// Count helpers (used by Statistics)
// ============================================================================
size_t RoutineManager::totalCount() const
{
    return routines_.size();
}

size_t RoutineManager::completedCount() const
{
    return static_cast<size_t>(
        std::count_if(routines_.begin(), routines_.end(),
                      [](const Routine& r) { return r.completed; }));
}

size_t RoutineManager::pendingCount() const
{
    return static_cast<size_t>(
        std::count_if(routines_.begin(), routines_.end(),
                      [](const Routine& r) { return !r.completed; }));
}

// ============================================================================
// Data access for JSON persistence
// ============================================================================
const std::vector<Routine>& RoutineManager::getRoutinesRef() const
{
    return routines_;
}

void RoutineManager::setRoutines(std::vector<Routine> new_routines)
{
    // std::move transfers the entire vector in O(1) - no element-by-element copy
    routines_ = std::move(new_routines);
    rebuildIndex();

    // Find the highest existing ID so new routines get unique IDs
    next_id_ = 1;
    for(const auto& r : routines_)
    {
        if(r.id >= next_id_)
        {
            next_id_ = r.id + 1;
        }
    }
}

void RoutineManager::clearAll()
{
    routines_.clear();
    id_index_.clear();
    next_id_ = 1;
}
