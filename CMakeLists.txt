#set(CMAKE_C_COMPILER gcc)
cmake_minimum_required(VERSION 3.17)
project(battleships)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(abseil-cpp)

add_executable(battleships
        main.cpp
            components/grid/GameGrid.cpp
            components/grid/GameGrid.h
    )
target_link_libraries(battleships absl::base absl::strings absl::str_format)
