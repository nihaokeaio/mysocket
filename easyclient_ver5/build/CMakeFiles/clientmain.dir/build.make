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
CMAKE_SOURCE_DIR = C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/build

# Include any dependencies generated for this target.
include CMakeFiles/clientmain.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/clientmain.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/clientmain.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/clientmain.dir/flags.make

CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.obj: CMakeFiles/clientmain.dir/flags.make
CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.obj: CMakeFiles/clientmain.dir/includes_CXX.rsp
CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.obj: C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/MyTCP_client_ver5/mainclient.cpp
CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.obj: CMakeFiles/clientmain.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.obj"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.obj -MF CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.obj.d -o CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.obj -c C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/MyTCP_client_ver5/mainclient.cpp

CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.i"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/MyTCP_client_ver5/mainclient.cpp > CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.i

CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.s"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/MyTCP_client_ver5/mainclient.cpp -o CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.s

CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.obj: CMakeFiles/clientmain.dir/flags.make
CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.obj: CMakeFiles/clientmain.dir/includes_CXX.rsp
CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.obj: C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/MyTCP_client_ver5/myclient.cpp
CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.obj: CMakeFiles/clientmain.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.obj"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.obj -MF CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.obj.d -o CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.obj -c C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/MyTCP_client_ver5/myclient.cpp

CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.i"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/MyTCP_client_ver5/myclient.cpp > CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.i

CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.s"
	C:/msys64/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/MyTCP_client_ver5/myclient.cpp -o CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.s

# Object files for target clientmain
clientmain_OBJECTS = \
"CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.obj" \
"CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.obj"

# External object files for target clientmain
clientmain_EXTERNAL_OBJECTS =

clientmain.exe: CMakeFiles/clientmain.dir/MyTCP_client_ver5/mainclient.cpp.obj
clientmain.exe: CMakeFiles/clientmain.dir/MyTCP_client_ver5/myclient.cpp.obj
clientmain.exe: CMakeFiles/clientmain.dir/build.make
clientmain.exe: CMakeFiles/clientmain.dir/linklibs.rsp
clientmain.exe: CMakeFiles/clientmain.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable clientmain.exe"
	"C:/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/clientmain.dir/objects.a
	C:/msys64/mingw64/bin/ar.exe qc CMakeFiles/clientmain.dir/objects.a @CMakeFiles/clientmain.dir/objects1.rsp
	C:/msys64/mingw64/bin/c++.exe -Wl,--whole-archive CMakeFiles/clientmain.dir/objects.a -Wl,--no-whole-archive -o clientmain.exe -Wl,--out-implib,libclientmain.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/clientmain.dir/linklibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/clientmain.dir/build: clientmain.exe
.PHONY : CMakeFiles/clientmain.dir/build

CMakeFiles/clientmain.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clientmain.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clientmain.dir/clean

CMakeFiles/clientmain.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5 C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5 C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/build C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/build C:/Users/ZQD-N/Desktop/program/mytest/SOCKET/easyclient_ver5/build/CMakeFiles/clientmain.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clientmain.dir/depend

