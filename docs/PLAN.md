# SnakePlus — Plan / Design

## Goals
1. Clean project structure (files, modules).
2. Main menu + navigation.
3. JSON settings persistence.
4. DB-backed high scores (SQLite) with schema creation on first run.
5. Simple but smooth gameplay.

## Screens / States
- MenuState:
  - Play
  - High Scores
  - Settings
  - Quit
- GameState:
  - Running
  - GameOver overlay
- ScoresState:
  - list of top scores from DB
- SettingsState:
  - speed, grid size, wrap walls on/off

## Data
- JSON (`data/settings.json`)
  - `speed_ms` (int)
  - `wrap_walls` (bool)
- SQLite DB (`data/scores.db`)
  - table `scores(id INTEGER PRIMARY KEY, name TEXT, score INTEGER, created_at TEXT)`

## Architecture
- App: window loop, state switch, resource init/shutdown.
- Menu: keyboard navigation, draws items.
- Game: owns World+Snake, updates, renders, handles collisions.
- Database: minimal wrapper, creates schema, inserts, fetches top.
- JsonMini: tiny JSON reader/writer for a small subset.

## Later upgrades
- Sound effects
- Animations (juice): screen fade, food pop, death flash
- Controller support
