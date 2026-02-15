# SnakePlus (C++17) — Files + JSON + DB + Main Menu

A small, clean Snake game project with:
- Main Menu (Play / High Scores / Settings / Quit)
- JSON settings file (no external JSON library, simple mini parser/writer)
- Score Database (SQLite, optional via CMake)
- Modular codebase (multiple .h/.cpp files)
- A short planning doc in `docs/PLAN.md`

## Requirements
- CMake 3.16+
- C++17 compiler
- SDL2 development package
- SQLite3 (optional, for DB scores)

## Build (Windows / Linux / macOS)
```bash
cmake -S . -B build
cmake --build build -j
```

Run:
```bash
./build/SnakePlus
```

## Notes
- Settings stored at: `data/settings.json`
- Scores DB stored at: `data/scores.db` (if SQLite is enabled)
- If SQLite is not found, the game still runs, and shows a helpful message in High Scores.

Enjoy 👾
