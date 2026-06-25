cmake_minimum_required(VERSION 3.20)
project(obs-football-plugin VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Find OBS ---
find_package(libobs REQUIRED)
find_package(obs-frontend-api REQUIRED)

# --- Find Qt6 ---
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# --- Find Dependencies ---
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# --- Sources ---
set(PLUGIN_SOURCES
    src/plugin-main.cpp
    src/football-source.cpp
    src/scoreboard.cpp
    src/timer.cpp
    src/api-football.cpp
    src/graphics/renderer.cpp
    src/graphics/scorebug.cpp
    src/graphics/lower-third.cpp
    src/graphics/transitions.cpp
    src/ui/control-dock.cpp
    src/ui/team-editor.cpp
    src/ui/settings-dialog.cpp
)

set(PLUGIN_HEADERS
    src/football-source.hpp
    src/scoreboard.hpp
    src/timer.hpp
    src/api-football.hpp
    src/graphics/renderer.hpp
    src/graphics/scorebug.hpp
    src/graphics/lower-third.hpp
    src/graphics/transitions.hpp
    src/ui/control-dock.hpp
)

# --- Build ---
add_library(obs-football-plugin MODULE
    ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS}
)

target_link_libraries(obs-football-plugin
    PRIVATE
        OBS::libobs
        OBS::obs-frontend-api
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

target_include_directories(obs-football-plugin
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# --- Install ---
install(TARGETS obs-football-plugin
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/obs-plugins/64bit"
)
install(DIRECTORY data/obs-plugins/obs-football
    DESTINATION "${CMAKE_INSTALL_PREFIX}/data/obs-plugins"
)
