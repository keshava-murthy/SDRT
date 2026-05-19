#pragma once

#include "routine.h"
#include <vector>          // C++11 STL: dynamic array (most-used container)
#include <unordered_map>   // C++11 STL: hash map for O(1) lookup by key
#include <functional>      // C++11: std::function - type-erased callable wrapper
#include <optional>        // C++17: optional return values

// ============================================================================
//   RoutineManager: handles all CRUD operations on routines
//   Uses STL containers: vector for ordered storage, unordered_map for fast lookup
// ============================================================================
class RoutineManager
{
private:
    // vector stores routines in insertion order
    std::vector<Routine> routines_;

    // hash map: ID -> vector index for O(1) lookup
    std::unordered_map<int, size_t> id_index_;

    int next_id_;

    void rebuildIndex();

public:
    RoutineManager();

    // --- CRUD Operations ---
    int  addRoutine(std::string name, std::string desc,
                    std::string category, Priority priority, int freq_hours = 24);
    bool deleteRoutine(int id);
    bool markCompleted(int id);
    bool updatePriority(int id, Priority new_priority);
    bool categorizeRoutine(int id, const std::string& category);

    // --- Query Operations ---
    std::vector<Routine> getAllRoutines() const;

    // C++11: std::function<bool(const Routine&)> accepts ANY callable:
    //   lambdas, function pointers, functors - very flexible filtering
    std::vector<Routine> filterRoutines(
        std::function<bool(const Routine&)> predicate) const;

    std::vector<Routine> getPendingRoutines() const;
    std::vector<Routine> getCompletedRoutines() const;
    std::vector<Routine> searchRoutines(const std::string& keyword) const;

    // C++17: std::optional - returns empty if routine not found (no exceptions)
    std::optional<Routine> getRoutineById(int id) const;

    // --- Counts ---
    size_t totalCount() const;
    size_t completedCount() const;
    size_t pendingCount() const;

    // --- For data persistence ---
    const std::vector<Routine>& getRoutinesRef() const;
    void setRoutines(std::vector<Routine> new_routines);
    void clearAll();
};
