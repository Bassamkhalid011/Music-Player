# DSA Music Player

A feature-rich desktop music player built in **C++** using **SFML**, demonstrating core Data Structures and Algorithms concepts through a real-world application. Includes user authentication, playlist management, and a graphical interface.

---

## Features

- User registration and login with **custom hashing** for password security
- Music playback powered by **SFML Audio**
- Songs stored and navigated using a **Doubly Linked List**
- Song search using a **Binary Search Tree (BST)**
- Playlist management with persistent file storage
- Graphical UI built with **SFML Graphics**
- MVC-inspired architecture separating logic and GUI

---

## Data Structures Used

| Structure | Purpose |
|---|---|
| Doubly Linked List | Stores songs in a playlist; enables prev/next navigation |
| Binary Search Tree | Fast song search by title |
| Custom Hash Function | Password hashing for user authentication |

---

## Prerequisites

| Requirement | Notes |
|---|---|
| Windows OS | Project built for Windows (x64) |
| SFML 2.x | Audio, Graphics, Window, System libraries |
| Visual Studio 2022 | With C++ Desktop Development workload |
| C++17 or later | Uses std::filesystem |

---

## Setup & Run

### Option 1 — Run directly (no build needed)

A prebuilt executable is included:

```
x64/Debug/DSAFINALLLL.exe
```

Just double-click it to run. All required SFML `.dll` files are included in the same folder.

---

### Option 2 — Build from source in Visual Studio

1. Open `DSAFINALLLL.slnx` in Visual Studio 2022
2. Make sure SFML libraries are linked in project properties
3. Select **x64 > Debug** configuration
4. Press **Ctrl + F5** to build and run

---

## Adding Music

Place your `.mp3` or `.wav` files inside the `assets/` folder:

```
assets/
├── song1.mp3
├── song2.mp3
```

The player will load songs from this folder automatically.

---

## Project Structure

```
DSAFINALLLL/
│
├── main.cpp                  → Entry point; initializes logic and GUI loop
├── music_player_logic.h      → Data structures, Song, Playlist, User classes
├── music_player_logic.cpp    → Implementation of all logic and DSA operations
├── music_player_gui.h        → GUI class declaration
├── music_player_gui.cpp.cpp  → SFML rendering and event handling
│
├── assets/                   → Put your music files here
│   └── *.mp3
│
├── users.txt                 → Stores registered users (hashed passwords)
├── Bassam_playlists.txt      → Stores saved playlists
│
├── x64/Debug/
│   └── DSAFINALLLL.exe       → Prebuilt executable
│
└── *.dll                     → SFML runtime libraries
```

---

## Technologies

- **Language:** C++ (C++17)
- **Graphics & Audio:** SFML 2.x
- **IDE:** Visual Studio 2022
- **Build System:** MSBuild (.vcxproj)
