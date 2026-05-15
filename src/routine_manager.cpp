#include "routine_manager.h"
#include <algorithm>  // for std::count_if

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
//   C++11: lock_guard - RAII-based locking
//     The mutex is automatically released when lock goes out of scope,
//     even if an exception is thrown. No risk of forgetting to unlock!
// ============================================================================
int RoutineManager::addRoutine(std::string name, std::string desc,
                                std::string category, Priority priority,
                                int freq_hours)
{
    std::lock_guard<std::mutex> lock(mtx_);

    int id = next_id_++;

    // C++11: emplace_back constructs the Routine directly inside the vector
    //   (avoids creating a temporary and then copying/moving it)
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
    std::lock_guard<std::mutex> lock(mtx_);

    auto it = id_index_.find(id);
    if(it == id_index_.end()) return false;

    routines_.erase(routines_.begin() + static_cast<long>(it->second));
    rebuildIndex();  // indices shifted, rebuild the map
    return true;
}

// ============================================================================
// Mark a routine as completed (records completion time)
// ============================================================================
bool RoutineManager::markCompleted(int id)
{
    std::lock_guard<std::mutex> lock(mtx_);

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
    std::lock_guard<std::mutex> lock(mtx_);

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
    std::lock_guard<std::mutex> lock(mtx_);

    auto it = id_index_.find(id);
    if(it == id_index_.end()) return false;

    routines_[it->second].category = category;
    return true;
}

// ============================================================================
// Get all routines (returns a copy - thread safe)
// ============================================================================
std::vector<Routine> RoutineManager::getAllRoutines() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return routines_;  // copy is intentional for thread safety
}

// ============================================================================
// Filter routines using a predicate (lambda, functor, or function pointer)
//
//   C++11: std::function<bool(const Routine&)> is a "function wrapper"
//     It can hold ANY callable matching that signature:
//       - Lambda:   filterRoutines([](const Routine& r){ return r.completed; })
//       - Function: filterRoutines(isCompleted)
//       - Functor:  filterRoutines(MyChecker{})
// ============================================================================
std::vector<Routine> RoutineManager::filterRoutines(
    std::function<bool(const Routine&)> predicate) const
{
    std::lock_guard<std::mutex> lock(mtx_);

    std::vector<Routine> result;

    // C++11: Range-based for loop - cleaner than index-based iteration
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
//   C++11: Lambda expressions - anonymous inline functions
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
    // C++11: Lambda captures 'keyword' by reference [&keyword]
    //   The lambda "remembers" the keyword variable from the outer scope
    return filterRoutines([&keyword](const Routine& r)
    {
        return r.name.find(keyword)        != std::string::npos
            || r.description.find(keyword) != std::string::npos
            || r.category.find(keyword)    != std::string::npos;
    });
}

// ============================================================================
// Get a single routine by ID
//   C++17: std::optional - "maybe it's there, maybe it's not"
//     Much better than returning a pointer or throwing an exception
// ============================================================================
std::optional<Routine> RoutineManager::getRoutineById(int id) const
{
    std::lock_guard<std::mutex> lock(mtx_);

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
    std::lock_guard<std::mutex> lock(mtx_);
    return routines_.size();
}

size_t RoutineManager::completedCount() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    // C++11: std::count_if with lambda - count elements matching a condition
    return static_cast<size_t>(
        std::count_if(routines_.begin(), routines_.end(),
                      [](const Routine& r) { return r.completed; }));
}

size_t RoutineManager::pendingCount() const
{
    std::lock_guard<std::mutex> lock(mtx_);
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
    std::lock_guard<std::mutex> lock(mtx_);

    // C++11: std::move transfers the entire vector in O(1)
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
    std::lock_guard<std::mutex> lock(mtx_);
    routines_.clear();
    id_index_.clear();
    next_id_ = 1;
}
