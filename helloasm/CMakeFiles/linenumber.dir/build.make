# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/timfame/cpp-course/helloasm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/timfame/cpp-course/helloasm

# Include any dependencies generated for this target.
include CMakeFiles/linenumber.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/linenumber.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/linenumber.dir/flags.make

CMakeFiles/linenumber.dir/linenumber.asm.o: CMakeFiles/linenumber.dir/flags.make
CMakeFiles/linenumber.dir/linenumber.asm.o: linenumber.asm
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/timfame/cpp-course/helloasm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building ASM object CMakeFiles/linenumber.dir/linenumber.asm.o"
	nasm -f elf64 -g -F dwarf -o CMakeFiles/linenumber.dir/linenumber.asm.o /home/timfame/cpp-course/helloasm/linenumber.asm

CMakeFiles/linenumber.dir/linenumber.asm.o.requires:

.PHONY : CMakeFiles/linenumber.dir/linenumber.asm.o.requires

CMakeFiles/linenumber.dir/linenumber.asm.o.provides: CMakeFiles/linenumber.dir/linenumber.asm.o.requires
	$(MAKE) -f CMakeFiles/linenumber.dir/build.make CMakeFiles/linenumber.dir/linenumber.asm.o.provides.build
.PHONY : CMakeFiles/linenumber.dir/linenumber.asm.o.provides

CMakeFiles/linenumber.dir/linenumber.asm.o.provides.build: CMakeFiles/linenumber.dir/linenumber.asm.o


# Object files for target linenumber
linenumber_OBJECTS = \
"CMakeFiles/linenumber.dir/linenumber.asm.o"

# External object files for target linenumber
linenumber_EXTERNAL_OBJECTS =

linenumber: CMakeFiles/linenumber.dir/linenumber.asm.o
linenumber: CMakeFiles/linenumber.dir/build.make
linenumber: CMakeFiles/linenumber.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/timfame/cpp-course/helloasm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking ASM executable linenumber"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/linenumber.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/linenumber.dir/build: linenumber

.PHONY : CMakeFiles/linenumber.dir/build

CMakeFiles/linenumber.dir/requires: CMakeFiles/linenumber.dir/linenumber.asm.o.requires

.PHONY : CMakeFiles/linenumber.dir/requires

CMakeFiles/linenumber.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/linenumber.dir/cmake_clean.cmake
.PHONY : CMakeFiles/linenumber.dir/clean

CMakeFiles/linenumber.dir/depend:
	cd /home/timfame/cpp-course/helloasm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/timfame/cpp-course/helloasm /home/timfame/cpp-course/helloasm /home/timfame/cpp-course/helloasm /home/timfame/cpp-course/helloasm /home/timfame/cpp-course/helloasm/CMakeFiles/linenumber.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/linenumber.dir/depend

