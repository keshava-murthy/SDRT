# Smart Daily Routine Tracker (SDRT)

A **Modern C++ console-based productivity system** where you can create daily routines, assign priorities, track progress, and manage your day -> all from the terminal.


## Rough design can be found here :
https://excalidraw.com/#json=c8JwvBUjznksa-HvseEh2,kXmWaAYHlnQQw6ThHBDR4w



---

## Quick Start

```bash
make          # build the project
make run      # run the application
```

**Requirements:** g++ 11+ with C++17 support


## Features

### 1. Routine Management
- **Add** routines with name, description, category, priority, and reminder frequency
- **View** all / pending / completed routines with color-coded display
- **Mark** routines as completed (with appreciation messages)
- **Update** priority, **categorize**, **delete**, or **search** routines

### 2. Statistics & Reports
- Total / Completed / Pending counts
- **Visual progress bar** with percentage
- Category-wise and Priority-wise breakdowns
- Appreciation message when you hit 100%

### 3. Data Management
- **Save** routines to a JSON file (`data/routines.json`)
- **Load** routines from the JSON file
- **Auto-save** on exit
- Clear all data with confirmation

### 4. Reminder Service (Coming Soon)
> The **Reminder Service** will be implemented in a future update using C++ threading
> concepts (`std::thread`, `std::mutex`, `std::condition_variable`, `std::atomic`).
> It will run a background thread to periodically check pending routines and notify
> the user with piled-up reminders.


## Detailed Flow

### Creating a Routine
1. User enters name, description, category, priority (1-4), and reminder frequency
2. Routine is stored in a `std::vector` with a unique auto-incrementing ID
3. An `unordered_map` index is maintained for O(1) lookup by ID

### Data Persistence
1. Routines are serialized to JSON using `nlohmann/json` (json.hpp)
2. The `data/` directory is auto-created using `std::filesystem`
3. On startup, saved data is loaded automatically
4. On exit, data is saved automatically

### Progress Tracking
1. The main menu shows a progress bar: `[########------] 57.0%`
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
