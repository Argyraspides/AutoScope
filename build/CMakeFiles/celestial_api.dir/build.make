# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/granicus/Desktop/celesital_api

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/granicus/Desktop/celesital_api/build

# Include any dependencies generated for this target.
include CMakeFiles/celestial_api.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/celestial_api.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/celestial_api.dir/flags.make

CMakeFiles/celestial_api.dir/src/celestial_api.cpp.o: CMakeFiles/celestial_api.dir/flags.make
CMakeFiles/celestial_api.dir/src/celestial_api.cpp.o: ../src/celestial_api.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/granicus/Desktop/celesital_api/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/celestial_api.dir/src/celestial_api.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/celestial_api.dir/src/celestial_api.cpp.o -c /home/granicus/Desktop/celesital_api/src/celestial_api.cpp

CMakeFiles/celestial_api.dir/src/celestial_api.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/celestial_api.dir/src/celestial_api.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/granicus/Desktop/celesital_api/src/celestial_api.cpp > CMakeFiles/celestial_api.dir/src/celestial_api.cpp.i

CMakeFiles/celestial_api.dir/src/celestial_api.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/celestial_api.dir/src/celestial_api.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/granicus/Desktop/celesital_api/src/celestial_api.cpp -o CMakeFiles/celestial_api.dir/src/celestial_api.cpp.s

# Object files for target celestial_api
celestial_api_OBJECTS = \
"CMakeFiles/celestial_api.dir/src/celestial_api.cpp.o"

# External object files for target celestial_api
celestial_api_EXTERNAL_OBJECTS =

celestial_api: CMakeFiles/celestial_api.dir/src/celestial_api.cpp.o
celestial_api: CMakeFiles/celestial_api.dir/build.make
celestial_api: curlpp/libcurlpp.so.1.0.0
celestial_api: /usr/lib/i386-linux-gnu/libjsoncpp.so.1.9.4
celestial_api: /usr/lib/i386-linux-gnu/libcurl.so
celestial_api: CMakeFiles/celestial_api.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/granicus/Desktop/celesital_api/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable celestial_api"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/celestial_api.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/celestial_api.dir/build: celestial_api

.PHONY : CMakeFiles/celestial_api.dir/build

CMakeFiles/celestial_api.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/celestial_api.dir/cmake_clean.cmake
.PHONY : CMakeFiles/celestial_api.dir/clean

CMakeFiles/celestial_api.dir/depend:
	cd /home/granicus/Desktop/celesital_api/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/granicus/Desktop/celesital_api /home/granicus/Desktop/celesital_api /home/granicus/Desktop/celesital_api/build /home/granicus/Desktop/celesital_api/build /home/granicus/Desktop/celesital_api/build/CMakeFiles/celestial_api.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/celestial_api.dir/depend

