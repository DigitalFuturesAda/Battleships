set(CMAKE_C_COMPILER gcc)
cmake_minimum_required(VERSION 3.10.2)
project(battleships)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(abseil-cpp)

add_executable(battleships
        main.cpp
        lib/tabulate.hpp
        components/grid/GameGrid.cpp components/grid/GameGrid.h components/util/strings.cpp components/util/strings.h components/player/Player.cpp components/player/Player.h components/ship/Ship.cpp components/ship/Ship.h components/grid/HitGrid.cpp components/grid/HitGrid.h components/util/io.cpp components/util/io.h components/util/rand.cpp components/util/rand.h components/player/HostController.cpp components/player/HostController.h components/player/GameFlowController.cpp components/player/GameFlowController.h components/config/ConfigFileParser.cpp components/config/ConfigFileParser.h components/config/ConfigValidator.cpp components/config/ConfigValidator.h components/config/ConfigSingleton.cpp components/config/ConfigSingleton.h components/menu/MenuHelper.cpp components/menu/MenuHelper.h)

target_link_libraries(battleships absl::base absl::str_format absl::flags)
set_target_properties(battleships PROPERTIES CXX_STANDARD 17)
