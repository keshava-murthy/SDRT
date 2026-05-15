#pragma once

#include "routine_manager.h"
#include <thread>              // C++11: std::thread for background work
#include <atomic>              // C++11: lock-free thread-safe variables
#include <condition_variable>  // C++11: thread signaling (sleep/wake pattern)
#include <functional>          // C++11: std::function (function wrappers)
#include <vector>
#include <string>

// ============================================================================
// C++11: Type alias with 'using' (modern replacement for typedef)
//   ReminderCallback is any callable that takes a Routine reference
//   This means lambdas, function pointers, or functors all work here
// ============================================================================
using ReminderCallback = std::function<void(const Routine&)>;

// ============================================================================
// ReminderService: runs a background thread that checks pending routines
//   - Notifications pile up until the user views them
//   - Once viewed, notifications disappear (as per design)
//   - Supports custom callbacks registered by the user
// ============================================================================
class ReminderService
{
private:
    RoutineManager& manager_;

    // C++11: std::thread - runs reminderLoop() in the background
    std::thread reminder_thread_;

    // C++11: std::atomic<bool> - thread-safe boolean, no mutex needed
    //   Multiple threads can read/write this safely
    std::atomic<bool> running_;

    // C++11: mutex + condition_variable for clean thread shutdown
    //   condition_variable lets the thread sleep but wake up instantly on stop()
    std::mutex mtx_;
    std::condition_variable cv_;

    // User-registered callbacks (invoked when reminders fire)
    std::vector<ReminderCallback> callbacks_;

    // Notifications pile up here until viewed
    std::vector<std::string> notifications_;
    mutable std::mutex notif_mtx_;

    // Background loop: checks routines periodically
    void reminderLoop();

public:
    explicit ReminderService(RoutineManager& mgr);
    ~ReminderService();

    void start();
    void stop();
    bool isRunning() const;

    // C++11: std::function parameter - accepts lambdas, functors, etc.
    void addCallback(ReminderCallback cb);

    // Show all piled-up notifications
    void executeNotifications();

    std::vector<std::string> getNotifications() const;
    void clearNotifications();
    size_t notificationCount() const;
};
