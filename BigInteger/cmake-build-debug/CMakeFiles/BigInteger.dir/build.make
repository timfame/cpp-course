# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /home/timfame/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/191.6183.77/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/timfame/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/191.6183.77/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/timfame/cpp-course/BigInteger

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/timfame/cpp-course/BigInteger/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/BigInteger.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/BigInteger.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BigInteger.dir/flags.make

CMakeFiles/BigInteger.dir/main.cpp.o: CMakeFiles/BigInteger.dir/flags.make
CMakeFiles/BigInteger.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/timfame/cpp-course/BigInteger/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/BigInteger.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BigInteger.dir/main.cpp.o -c /home/timfame/cpp-course/BigInteger/main.cpp

CMakeFiles/BigInteger.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BigInteger.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/timfame/cpp-course/BigInteger/main.cpp > CMakeFiles/BigInteger.dir/main.cpp.i

CMakeFiles/BigInteger.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BigInteger.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/timfame/cpp-course/BigInteger/main.cpp -o CMakeFiles/BigInteger.dir/main.cpp.s

CMakeFiles/BigInteger.dir/big_integer.cpp.o: CMakeFiles/BigInteger.dir/flags.make
CMakeFiles/BigInteger.dir/big_integer.cpp.o: ../big_integer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/timfame/cpp-course/BigInteger/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/BigInteger.dir/big_integer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BigInteger.dir/big_integer.cpp.o -c /home/timfame/cpp-course/BigInteger/big_integer.cpp

CMakeFiles/BigInteger.dir/big_integer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BigInteger.dir/big_integer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/timfame/cpp-course/BigInteger/big_integer.cpp > CMakeFiles/BigInteger.dir/big_integer.cpp.i

CMakeFiles/BigInteger.dir/big_integer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BigInteger.dir/big_integer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/timfame/cpp-course/BigInteger/big_integer.cpp -o CMakeFiles/BigInteger.dir/big_integer.cpp.s

# Object files for target BigInteger
BigInteger_OBJECTS = \
"CMakeFiles/BigInteger.dir/main.cpp.o" \
"CMakeFiles/BigInteger.dir/big_integer.cpp.o"

# External object files for target BigInteger
BigInteger_EXTERNAL_OBJECTS =

BigInteger: CMakeFiles/BigInteger.dir/main.cpp.o
BigInteger: CMakeFiles/BigInteger.dir/big_integer.cpp.o
BigInteger: CMakeFiles/BigInteger.dir/build.make
BigInteger: CMakeFiles/BigInteger.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/timfame/cpp-course/BigInteger/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable BigInteger"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BigInteger.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BigInteger.dir/build: BigInteger

.PHONY : CMakeFiles/BigInteger.dir/build

CMakeFiles/BigInteger.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BigInteger.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BigInteger.dir/clean

CMakeFiles/BigInteger.dir/depend:
	cd /home/timfame/cpp-course/BigInteger/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/timfame/cpp-course/BigInteger /home/timfame/cpp-course/BigInteger /home/timfame/cpp-course/BigInteger/cmake-build-debug /home/timfame/cpp-course/BigInteger/cmake-build-debug /home/timfame/cpp-course/BigInteger/cmake-build-debug/CMakeFiles/BigInteger.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BigInteger.dir/depend
