set(CMAKE_C_COMPILER gcc)
cmake_minimum_required(VERSION 3.17)
project(battleships)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(abseil-cpp)

add_executable(battleships
        tabulate.hpp
        main.cpp components/grid/GameGrid.cpp components/grid/GameGrid.h
        components/util/strings.cpp components/util/strings.h components/player/Player.cpp components/player/Player.h components/ship/Ship.cpp components/ship/Ship.h)

target_link_libraries(battleships
        absl::base absl::strings absl::str_format)
