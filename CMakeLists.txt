cmake_minimum_required(VERSION 3.16)
project(SnakePlusCPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SNAKEPLUS_WITH_SQLITE "Enable SQLite score database" ON)

# --- SDL2 ---
find_package(SDL2 REQUIRED)

add_executable(SnakePlus
    src/main.cpp
    src/app/App.cpp
    src/app/App.h
    src/core/Config.h
    src/core/Types.h
    src/core/Timer.h
    src/core/Timer.cpp
    src/io/JsonMini.h
    src/io/JsonMini.cpp
    src/db/Database.h
    src/db/Database.cpp
    src/game/Game.cpp
    src/game/Game.h
    src/game/Menu.cpp
    src/game/Menu.h
    src/game/Snake.cpp
    src/game/Snake.h
    src/game/World.cpp
    src/game/World.h
    src/ui/Draw.cpp
    src/ui/Draw.h
)

target_include_directories(SnakePlus PRIVATE src)
target_link_libraries(SnakePlus PRIVATE SDL2::SDL2 SDL2::SDL2main)

if (SNAKEPLUS_WITH_SQLITE)
    find_package(SQLite3)
    if (SQLite3_FOUND)
        target_compile_definitions(SnakePlus PRIVATE SNAKEPLUS_WITH_SQLITE=1)
        target_link_libraries(SnakePlus PRIVATE SQLite::SQLite3)
    else()
        message(WARNING "SQLite3 not found. Building without DB support.")
        target_compile_definitions(SnakePlus PRIVATE SNAKEPLUS_WITH_SQLITE=0)
    endif()
else()
    target_compile_definitions(SnakePlus PRIVATE SNAKEPLUS_WITH_SQLITE=0)
endif()
