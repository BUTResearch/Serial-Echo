# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/student/Desktop/Serial-Echo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/student/Desktop/Serial-Echo/build

# Include any dependencies generated for this target.
include Libs/Modem/CMakeFiles/sara_n210_lib.dir/depend.make

# Include the progress variables for this target.
include Libs/Modem/CMakeFiles/sara_n210_lib.dir/progress.make

# Include the compile flags for this target's objects.
include Libs/Modem/CMakeFiles/sara_n210_lib.dir/flags.make

Libs/Modem/CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.o: Libs/Modem/CMakeFiles/sara_n210_lib.dir/flags.make
Libs/Modem/CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.o: ../Libs/Modem/sara_n210.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/student/Desktop/Serial-Echo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Libs/Modem/CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.o"
	cd /home/student/Desktop/Serial-Echo/build/Libs/Modem && /bin/g++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.o -c /home/student/Desktop/Serial-Echo/Libs/Modem/sara_n210.cpp

Libs/Modem/CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.i"
	cd /home/student/Desktop/Serial-Echo/build/Libs/Modem && /bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/student/Desktop/Serial-Echo/Libs/Modem/sara_n210.cpp > CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.i

Libs/Modem/CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.s"
	cd /home/student/Desktop/Serial-Echo/build/Libs/Modem && /bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/student/Desktop/Serial-Echo/Libs/Modem/sara_n210.cpp -o CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.s

# Object files for target sara_n210_lib
sara_n210_lib_OBJECTS = \
"CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.o"

# External object files for target sara_n210_lib
sara_n210_lib_EXTERNAL_OBJECTS =

Libs/Modem/libsara_n210_lib.a: Libs/Modem/CMakeFiles/sara_n210_lib.dir/sara_n210.cpp.o
Libs/Modem/libsara_n210_lib.a: Libs/Modem/CMakeFiles/sara_n210_lib.dir/build.make
Libs/Modem/libsara_n210_lib.a: Libs/Modem/CMakeFiles/sara_n210_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/student/Desktop/Serial-Echo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libsara_n210_lib.a"
	cd /home/student/Desktop/Serial-Echo/build/Libs/Modem && $(CMAKE_COMMAND) -P CMakeFiles/sara_n210_lib.dir/cmake_clean_target.cmake
	cd /home/student/Desktop/Serial-Echo/build/Libs/Modem && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sara_n210_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Libs/Modem/CMakeFiles/sara_n210_lib.dir/build: Libs/Modem/libsara_n210_lib.a

.PHONY : Libs/Modem/CMakeFiles/sara_n210_lib.dir/build

Libs/Modem/CMakeFiles/sara_n210_lib.dir/clean:
	cd /home/student/Desktop/Serial-Echo/build/Libs/Modem && $(CMAKE_COMMAND) -P CMakeFiles/sara_n210_lib.dir/cmake_clean.cmake
.PHONY : Libs/Modem/CMakeFiles/sara_n210_lib.dir/clean

Libs/Modem/CMakeFiles/sara_n210_lib.dir/depend:
	cd /home/student/Desktop/Serial-Echo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/student/Desktop/Serial-Echo /home/student/Desktop/Serial-Echo/Libs/Modem /home/student/Desktop/Serial-Echo/build /home/student/Desktop/Serial-Echo/build/Libs/Modem /home/student/Desktop/Serial-Echo/build/Libs/Modem/CMakeFiles/sara_n210_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Libs/Modem/CMakeFiles/sara_n210_lib.dir/depend

