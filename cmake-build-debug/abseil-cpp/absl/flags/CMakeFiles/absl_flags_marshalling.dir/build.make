# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/slyo/CLionProjects/battleships

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/slyo/CLionProjects/battleships/cmake-build-debug

# Include any dependencies generated for this target.
include abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/depend.make

# Include the progress variables for this target.
include abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/progress.make

# Include the compile flags for this target's objects.
include abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/flags.make

abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.o: abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/flags.make
abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.o: ../abseil-cpp/absl/flags/marshalling.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/slyo/CLionProjects/battleships/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.o"
	cd /Users/slyo/CLionProjects/battleships/cmake-build-debug/abseil-cpp/absl/flags && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.o -c /Users/slyo/CLionProjects/battleships/abseil-cpp/absl/flags/marshalling.cc

abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.i"
	cd /Users/slyo/CLionProjects/battleships/cmake-build-debug/abseil-cpp/absl/flags && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/slyo/CLionProjects/battleships/abseil-cpp/absl/flags/marshalling.cc > CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.i

abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.s"
	cd /Users/slyo/CLionProjects/battleships/cmake-build-debug/abseil-cpp/absl/flags && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/slyo/CLionProjects/battleships/abseil-cpp/absl/flags/marshalling.cc -o CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.s

# Object files for target absl_flags_marshalling
absl_flags_marshalling_OBJECTS = \
"CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.o"

# External object files for target absl_flags_marshalling
absl_flags_marshalling_EXTERNAL_OBJECTS =

abseil-cpp/absl/flags/libabsl_flags_marshalling.a: abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/marshalling.cc.o
abseil-cpp/absl/flags/libabsl_flags_marshalling.a: abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/build.make
abseil-cpp/absl/flags/libabsl_flags_marshalling.a: abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/slyo/CLionProjects/battleships/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libabsl_flags_marshalling.a"
	cd /Users/slyo/CLionProjects/battleships/cmake-build-debug/abseil-cpp/absl/flags && $(CMAKE_COMMAND) -P CMakeFiles/absl_flags_marshalling.dir/cmake_clean_target.cmake
	cd /Users/slyo/CLionProjects/battleships/cmake-build-debug/abseil-cpp/absl/flags && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/absl_flags_marshalling.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/build: abseil-cpp/absl/flags/libabsl_flags_marshalling.a

.PHONY : abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/build

abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/clean:
	cd /Users/slyo/CLionProjects/battleships/cmake-build-debug/abseil-cpp/absl/flags && $(CMAKE_COMMAND) -P CMakeFiles/absl_flags_marshalling.dir/cmake_clean.cmake
.PHONY : abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/clean

abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/depend:
	cd /Users/slyo/CLionProjects/battleships/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/slyo/CLionProjects/battleships /Users/slyo/CLionProjects/battleships/abseil-cpp/absl/flags /Users/slyo/CLionProjects/battleships/cmake-build-debug /Users/slyo/CLionProjects/battleships/cmake-build-debug/abseil-cpp/absl/flags /Users/slyo/CLionProjects/battleships/cmake-build-debug/abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : abseil-cpp/absl/flags/CMakeFiles/absl_flags_marshalling.dir/depend

