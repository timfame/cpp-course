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
CMAKE_SOURCE_DIR = /home/timfame/cpp-course/testBig

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/timfame/cpp-course/testBig/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/testBig.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testBig.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testBig.dir/flags.make

CMakeFiles/testBig.dir/main.cpp.o: CMakeFiles/testBig.dir/flags.make
CMakeFiles/testBig.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/timfame/cpp-course/testBig/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testBig.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testBig.dir/main.cpp.o -c /home/timfame/cpp-course/testBig/main.cpp

CMakeFiles/testBig.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testBig.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/timfame/cpp-course/testBig/main.cpp > CMakeFiles/testBig.dir/main.cpp.i

CMakeFiles/testBig.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testBig.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/timfame/cpp-course/testBig/main.cpp -o CMakeFiles/testBig.dir/main.cpp.s

CMakeFiles/testBig.dir/bit_vector.cpp.o: CMakeFiles/testBig.dir/flags.make
CMakeFiles/testBig.dir/bit_vector.cpp.o: ../bit_vector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/timfame/cpp-course/testBig/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/testBig.dir/bit_vector.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testBig.dir/bit_vector.cpp.o -c /home/timfame/cpp-course/testBig/bit_vector.cpp

CMakeFiles/testBig.dir/bit_vector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testBig.dir/bit_vector.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/timfame/cpp-course/testBig/bit_vector.cpp > CMakeFiles/testBig.dir/bit_vector.cpp.i

CMakeFiles/testBig.dir/bit_vector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testBig.dir/bit_vector.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/timfame/cpp-course/testBig/bit_vector.cpp -o CMakeFiles/testBig.dir/bit_vector.cpp.s

CMakeFiles/testBig.dir/big_integer.cpp.o: CMakeFiles/testBig.dir/flags.make
CMakeFiles/testBig.dir/big_integer.cpp.o: ../big_integer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/timfame/cpp-course/testBig/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/testBig.dir/big_integer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testBig.dir/big_integer.cpp.o -c /home/timfame/cpp-course/testBig/big_integer.cpp

CMakeFiles/testBig.dir/big_integer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testBig.dir/big_integer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/timfame/cpp-course/testBig/big_integer.cpp > CMakeFiles/testBig.dir/big_integer.cpp.i

CMakeFiles/testBig.dir/big_integer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testBig.dir/big_integer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/timfame/cpp-course/testBig/big_integer.cpp -o CMakeFiles/testBig.dir/big_integer.cpp.s

# Object files for target testBig
testBig_OBJECTS = \
"CMakeFiles/testBig.dir/main.cpp.o" \
"CMakeFiles/testBig.dir/bit_vector.cpp.o" \
"CMakeFiles/testBig.dir/big_integer.cpp.o"

# External object files for target testBig
testBig_EXTERNAL_OBJECTS =

testBig: CMakeFiles/testBig.dir/main.cpp.o
testBig: CMakeFiles/testBig.dir/bit_vector.cpp.o
testBig: CMakeFiles/testBig.dir/big_integer.cpp.o
testBig: CMakeFiles/testBig.dir/build.make
testBig: CMakeFiles/testBig.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/timfame/cpp-course/testBig/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable testBig"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testBig.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testBig.dir/build: testBig

.PHONY : CMakeFiles/testBig.dir/build

CMakeFiles/testBig.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testBig.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testBig.dir/clean

CMakeFiles/testBig.dir/depend:
	cd /home/timfame/cpp-course/testBig/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/timfame/cpp-course/testBig /home/timfame/cpp-course/testBig /home/timfame/cpp-course/testBig/cmake-build-debug /home/timfame/cpp-course/testBig/cmake-build-debug /home/timfame/cpp-course/testBig/cmake-build-debug/CMakeFiles/testBig.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testBig.dir/depend

