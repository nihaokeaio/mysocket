# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/build

# Include any dependencies generated for this target.
include CMakeFiles/servermain.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/servermain.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/servermain.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/servermain.dir/flags.make

CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.obj: CMakeFiles/servermain.dir/flags.make
CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.obj: CMakeFiles/servermain.dir/includes_CXX.rsp
CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.obj: C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/MyTCP_server_ver3/mainserver.cpp
CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.obj: CMakeFiles/servermain.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.obj"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.obj -MF CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.obj.d -o CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.obj -c C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/MyTCP_server_ver3/mainserver.cpp

CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.i"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/MyTCP_server_ver3/mainserver.cpp > CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.i

CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.s"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/MyTCP_server_ver3/mainserver.cpp -o CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.s

CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.obj: CMakeFiles/servermain.dir/flags.make
CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.obj: CMakeFiles/servermain.dir/includes_CXX.rsp
CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.obj: C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/MyTCP_server_ver3/myserver.cpp
CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.obj: CMakeFiles/servermain.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.obj"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.obj -MF CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.obj.d -o CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.obj -c C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/MyTCP_server_ver3/myserver.cpp

CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.i"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/MyTCP_server_ver3/myserver.cpp > CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.i

CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.s"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/MyTCP_server_ver3/myserver.cpp -o CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.s

# Object files for target servermain
servermain_OBJECTS = \
"CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.obj" \
"CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.obj"

# External object files for target servermain
servermain_EXTERNAL_OBJECTS =

servermain.exe: CMakeFiles/servermain.dir/MyTCP_server_ver3/mainserver.cpp.obj
servermain.exe: CMakeFiles/servermain.dir/MyTCP_server_ver3/myserver.cpp.obj
servermain.exe: CMakeFiles/servermain.dir/build.make
servermain.exe: CMakeFiles/servermain.dir/linklibs.rsp
servermain.exe: CMakeFiles/servermain.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable servermain.exe"
	"C:/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/servermain.dir/objects.a
	C:/msys64/mingw64/bin/ar.exe qc CMakeFiles/servermain.dir/objects.a @CMakeFiles/servermain.dir/objects1.rsp
	C:/msys64/mingw64/bin/c++.exe -Wl,--whole-archive CMakeFiles/servermain.dir/objects.a -Wl,--no-whole-archive -o servermain.exe -Wl,--out-implib,libservermain.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/servermain.dir/linklibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/servermain.dir/build: servermain.exe
.PHONY : CMakeFiles/servermain.dir/build

CMakeFiles/servermain.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/servermain.dir/cmake_clean.cmake
.PHONY : CMakeFiles/servermain.dir/clean

CMakeFiles/servermain.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3 C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3 C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/build C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/build C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver3/build/CMakeFiles/servermain.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/servermain.dir/depend

