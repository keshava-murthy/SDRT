# Smart Daily Routine Tracker (SDRT)

A **Modern C++ console-based productivity system** where you can create daily routines, assign priorities, track progress, and get reminders — all from the terminal.

Think of it as a simplified **Todoist** or **Google Tasks** in the terminal.

---

## Quick Start

```bash
cd training/sdrt
make          # build the project
make run      # run the application
```

**Requirements:** g++ 11+ with C++17 support, pthreads (standard on Linux/Mac)

---

## Application Flow

```
┌─────────────────────────────────────┐
│            STARTUP                  │
│  - Load saved data from JSON file   │
│  - Show a motivational quote        │
│  - Display quick stats & progress   │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│          MAIN MENU                  │
│                                     │
│  [########------] 55.0%             │
│  3 done | 2 pending | 5 total      │
│  >> 4 notification(s) <<           │
│                                     │
│  1. Routine Management              │
│  2. Reminder Management             │
│  3. Statistics & Reports            │
│  4. Data Management                 │
│  0. Exit (auto-saves)               │
└─────────────────────────────────────┘
        │         │        │        │
        ▼         ▼        ▼        ▼
   ┌────────┐ ┌───────┐ ┌──────┐ ┌──────┐
   │Routines│ │Remind-│ │Stats │ │Data  │
   │  CRUD  │ │  ers  │ │Report│ │Save/ │
   │        │ │Thread │ │      │ │Load  │
   └────────┘ └───────┘ └──────┘ └──────┘
```

---

## Features

### 1. Routine Management
- **Add** routines with name, description, category, priority, and reminder frequency
- **View** all / pending / completed routines with color-coded display
- **Mark** routines as completed (with appreciation messages!)
- **Update** priority, **categorize**, **delete**, or **search** routines

### 2. Reminder Management
- **Background thread** checks pending routines every 30 seconds
- Notifications **pile up** until you view them
- Once viewed, notifications **disappear** (as designed)
- Register custom **callbacks** (lambda functions) for reminders

### 3. Statistics & Reports
- Total / Completed / Pending counts
- **Visual progress bar** with percentage
- Category-wise and Priority-wise breakdowns
- Appreciation message when you hit 100%!

### 4. Data Management
- **Save** routines to a JSON file (`data/routines.json`)
- **Load** routines from the JSON file
- **Auto-save** on exit
- Clear all data with confirmation

### Bonus Features
- Random **motivational quote** on startup
- **Appreciation messages** and real-world benefit tips when completing routines
- **Stylish console** with colors and formatting (ANSI escape codes)
- Cross-platform terminal compatibility (Linux, Mac, Windows 10+)

---

## Project Structure

```
sdrt/
├── Makefile              # Build configuration
├── README.md             # This file
├── include/              # Header files (declarations)
│   ├── routine.h         # Routine data model
│   ├── routine_manager.h # CRUD operations
│   ├── reminder_service.h# Background reminder thread
│   ├── statistics.h      # Reports & analytics
│   ├── data_manager.h    # JSON persistence
│   ├── display.h         # Console UI (colors, progress bar)
│   ├── quotes.h          # Motivational quotes
│   └── menu.h            # Interactive menu system
├── src/                  # Source files (implementations)
│   ├── main.cpp          # Entry point
│   ├── routine.cpp       # Routine implementation
│   ├── routine_manager.cpp
│   ├── reminder_service.cpp
│   ├── statistics.cpp
│   ├── data_manager.cpp
│   ├── display.cpp
│   ├── quotes.cpp
│   └── menu.cpp
└── data/                 # Created at runtime
    └── routines.json     # Saved routine data
```

---

## C++ Concepts Used (mapped to training syllabus)

| Training Day | Concept | Where Used |
|---|---|---|
| Day 1 | **STL Containers** (vector, map, unordered_map) | RoutineManager, Statistics |
| Day 1 | **std::pair** | Quotes benefit messages |
| Day 1 | **constexpr** | ANSI color codes in display.h |
| Day 1 | **Lambda Expressions** | Filtering routines, callbacks |
| Day 1 | **Functors / Function Wrappers** | ReminderCallback (std::function) |
| Day 2 | **Lambda Captures** | Reminder callbacks, search filter |
| Day 2 | **std::initializer_list** | Vector initialization with {} |
| Day 2 | **Move Semantics** | String transfers in constructors |
| Day 2 | **Move Assignment** | Vector moves in data loading |
| Day 3 | **Range-based for loops** | Used everywhere |
| Day 3 | **enum class** | Priority (LOW, MEDIUM, HIGH, CRITICAL) |
| Day 4 | **Smart Pointers** (unique_ptr) | main.cpp object ownership |
| Day 4 | **Structured Bindings** | Map iteration in reports, quotes |
| Day 4 | **std::optional** | getRoutineById, reminder_time |
| Day 4 | **std::chrono** | Timestamps, thread sleep intervals |
| Day 5 | **std::thread** | ReminderService background thread |
| Day 5 | **std::mutex / lock_guard** | Thread-safe routine access |
| Day 5 | **std::condition_variable** | Interruptible sleep in reminder |
| Day 5 | **std::atomic** | Thread-safe running flag |

---

## How It Works (Detailed Flow)

### Creating a Routine
1. User enters name, description, category, priority (1-4), and reminder frequency
2. Routine is stored in a `std::vector` with a unique auto-incrementing ID
3. An `unordered_map` index is maintained for O(1) lookup by ID

### Reminder Thread
1. When started, a `std::thread` runs a loop in the background
2. Every 30 seconds, it checks all pending routines
3. For each pending routine, a notification string is generated and stored
4. Notifications pile up in a vector, protected by a mutex
5. When the user views notifications, they are displayed and cleared
6. The thread uses `condition_variable::wait_for` so it can be stopped instantly

### Data Persistence
1. Routines are serialized to JSON using `nlohmann/json` (json.hpp)
2. The `data/` directory is auto-created using `std::filesystem`
3. On startup, saved data is loaded automatically
4. On exit, data is saved automatically

### Progress Tracking
1. The main menu shows a progress bar: `[########------] 55.0%`
2. Percentage = completed routines / total routines * 100
3. When hitting 100%, a special appreciation message is shown

---

## Build Commands

```bash
make          # Compile the project
make run      # Compile and run
make clean    # Remove build artifacts
```

---

## JSON Data Format

Saved in `data/routines.json`:

```json
{
    "routines": [
        {
            "id": 1,
            "name": "Morning Exercise",
            "description": "30 min jog in the park",
            "category": "Health",
            "priority": 3,
            "completed": false,
            "created_at": "2026-05-15 09:00:00",
            "completed_at": "",
            "notification_freq_hours": 24,
            "reminder_time": null
        }
    ]
}
```

---

## Timeline (7-day plan)

| Day | Task |
|-----|------|
| 1 | Project setup, Routine model, RoutineManager (CRUD) |
| 2 | Display utilities (colors, progress bar), Menu system |
| 3 | Reminder Service (threading, notifications) |
| 4 | Statistics & Reports, Motivational quotes |
| 5 | Data persistence (JSON save/load) |
| 6 | Testing, bug fixes, polish |
| 7 | Documentation, code review, presentation prep |
