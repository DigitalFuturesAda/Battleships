set(CMAKE_C_COMPILER gcc)
cmake_minimum_required(VERSION 3.17)
project(battleships)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(abseil-cpp)

# FTXUI
include(FetchContent)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        )
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# ^ FTXUI

add_executable(battleships
        lib/tabulate.hpp
        main.cpp components/grid/GameGrid.cpp components/grid/GameGrid.h
        components/util/strings.cpp components/util/strings.h components/player/Player.cpp components/player/Player.h components/ship/Ship.cpp components/ship/Ship.h components/grid/HitGrid.cpp components/grid/HitGrid.h components/util/io.cpp components/util/io.h components/util/rand.cpp components/util/rand.h)

target_link_libraries(battleships
        absl::base absl::strings absl::str_format
        ftxui::screen ftxui::component
        )
set_target_properties(battleships PROPERTIES CXX_STANDARD 17)
