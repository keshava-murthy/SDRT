#include "reminder_service.h"
#include "display.h"
#include <iostream>
#include <sstream>
#include <chrono>

// ============================================================================
// Constructor: initialize with a reference to the shared RoutineManager
// ============================================================================
ReminderService::ReminderService(RoutineManager& mgr)
    : manager_(mgr)
    , running_(false)  // C++11: atomic<bool> initialization
{
}

// ============================================================================
// Destructor: ensure the background thread is stopped
// ============================================================================
ReminderService::~ReminderService()
{
    stop();
}

// ============================================================================
// Start the background reminder thread
//   C++11: std::thread - creates an OS thread running reminderLoop()
// ============================================================================
void ReminderService::start()
{
    // C++11: atomic::load - safe read across threads
    if(running_.load()) return;  // already running

    running_.store(true);

    // C++11: Lambda captured 'this' to call member function in new thread
    reminder_thread_ = std::thread([this]() { reminderLoop(); });
}

// ============================================================================
// Stop the reminder thread gracefully
//   Uses condition_variable to wake the sleeping thread immediately
// ============================================================================
void ReminderService::stop()
{
    if(!running_.load()) return;

    // C++11: atomic::store - thread-safe write
    running_.store(false);

    // C++11: condition_variable::notify_one - wake up the sleeping thread
    cv_.notify_one();

    // Wait for the thread to finish (join = "wait until thread exits")
    if(reminder_thread_.joinable())
    {
        reminder_thread_.join();
    }
}

bool ReminderService::isRunning() const
{
    return running_.load();
}

// ============================================================================
// Register a callback that fires when reminders are generated
//   C++11: std::function + std::move = efficient callable storage
// ============================================================================
void ReminderService::addCallback(ReminderCallback cb)
{
    std::lock_guard<std::mutex> lock(mtx_);
    callbacks_.push_back(std::move(cb));
}

// ============================================================================
// Background loop: checks pending routines every 30 seconds
//   (30s is for demo; in production you'd use hours)
//
//   Key threading concepts used:
//     - mutex + lock_guard for safe data access
//     - condition_variable::wait_for for interruptible sleep
//     - atomic<bool> for the running flag
// ============================================================================
void ReminderService::reminderLoop()
{
    while(running_.load())
    {
        // Get pending routines (thread-safe via RoutineManager's own mutex)
        auto pending = manager_.getPendingRoutines();

        if(!pending.empty())
        {
            // Step 1: Build notification strings
            std::vector<std::string> new_notifs;
            for(const auto& routine : pending)
            {
                std::stringstream ss;
                ss << "[Reminder] \"" << routine.name << "\" is still pending! "
                   << "(Priority: " << Routine::priorityToString(routine.priority) << ")";
                new_notifs.push_back(ss.str());
            }

            // Step 2: Add notifications (lock only notif_mtx_)
            {
                std::lock_guard<std::mutex> lock(notif_mtx_);
                for(auto& n : new_notifs)
                {
                    notifications_.push_back(std::move(n));
                }
            }

            // Step 3: Execute callbacks (lock only mtx_)
            {
                std::lock_guard<std::mutex> lock(mtx_);
                for(const auto& routine : pending)
                {
                    for(const auto& callback : callbacks_)
                    {
                        callback(routine);
                    }
                }
            }
        }

        // C++11: condition_variable::wait_for
        //   Sleeps for 30 seconds BUT wakes up immediately if stop() is called
        //   The lambda predicate prevents spurious wakeups
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait_for(lock, std::chrono::seconds(30), [this]()
        {
            return !running_.load();  // wake up early if stopping
        });
    }
}

// ============================================================================
// Display all piled-up notifications
//   Design: notifications pile up; once viewed, user can clear them
// ============================================================================
void ReminderService::executeNotifications()
{
    std::lock_guard<std::mutex> lock(notif_mtx_);

    if(notifications_.empty())
    {
        Display::printInfo("No new notifications.");
        return;
    }

    Display::printHeader("NOTIFICATIONS");
    for(size_t i = 0; i < notifications_.size(); ++i)
    {
        std::cout << Color::YELLOW << "  " << (i + 1) << ". "
                  << notifications_[i] << Color::RESET << "\n";
    }
    std::cout << "\n";
    Display::printInfo("Showing " + std::to_string(notifications_.size()) + " notification(s).");
}

std::vector<std::string> ReminderService::getNotifications() const
{
    std::lock_guard<std::mutex> lock(notif_mtx_);
    return notifications_;
}

void ReminderService::clearNotifications()
{
    std::lock_guard<std::mutex> lock(notif_mtx_);
    notifications_.clear();
}

size_t ReminderService::notificationCount() const
{
    std::lock_guard<std::mutex> lock(notif_mtx_);
    return notifications_.size();
}
