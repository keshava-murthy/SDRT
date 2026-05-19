# Smart Daily Routine Tracker (SDRT)

A **Modern C++ console-based productivity system** where you can create daily routines, assign priorities, track progress, and get reminders — all from the terminal.

---

## Quick Start

```bash
make          # build the project
make run      # run the application
```

**Requirements:** g++ 11+ with C++17 support, pthreads (standard on Linux/Mac)


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
- Appreciation message when you hit 100%

### 4. Data Management
- **Save** routines to a JSON file (`data/routines.json`)
- **Load** routines from the JSON file
- **Auto-save** on exit
- Clear all data with confirmation


## Detailed Flow

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

## Rough design can be found here :
https://excalidraw.com/#json=afIo6VsZ-XbmuGbJoASD9,ZgjCb_iDKzgU_UjVNIO3gw

---
