# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/app/project/test/cpp/alg

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/app/project/test/cpp/alg/build

# Include any dependencies generated for this target.
include samples/CMakeFiles/test_dbr_recoall_server.bin.dir/depend.make

# Include the progress variables for this target.
include samples/CMakeFiles/test_dbr_recoall_server.bin.dir/progress.make

# Include the compile flags for this target's objects.
include samples/CMakeFiles/test_dbr_recoall_server.bin.dir/flags.make

samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o: samples/CMakeFiles/test_dbr_recoall_server.bin.dir/flags.make
samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o: ../samples/dbr_recoall_server.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/app/project/test/cpp/alg/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o"
	cd /home/app/project/test/cpp/alg/build/samples && /bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o -c /home/app/project/test/cpp/alg/samples/dbr_recoall_server.cpp

samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.i"
	cd /home/app/project/test/cpp/alg/build/samples && /bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/app/project/test/cpp/alg/samples/dbr_recoall_server.cpp > CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.i

samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.s"
	cd /home/app/project/test/cpp/alg/build/samples && /bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/app/project/test/cpp/alg/samples/dbr_recoall_server.cpp -o CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.s

samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o.requires:
.PHONY : samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o.requires

samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o.provides: samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o.requires
	$(MAKE) -f samples/CMakeFiles/test_dbr_recoall_server.bin.dir/build.make samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o.provides.build
.PHONY : samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o.provides

samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o.provides.build: samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o

# Object files for target test_dbr_recoall_server.bin
test_dbr_recoall_server_bin_OBJECTS = \
"CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o"

# External object files for target test_dbr_recoall_server.bin
test_dbr_recoall_server_bin_EXTERNAL_OBJECTS =

samples/test_dbr_recoall_server.bin: samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o
samples/test_dbr_recoall_server.bin: samples/CMakeFiles/test_dbr_recoall_server.bin.dir/build.make
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_videostab.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_video.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_ts.a
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_superres.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_stitching.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_photo.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_objdetect.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_nonfree.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_ml.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_legacy.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_imgproc.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_highgui.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_gpu.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_flann.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_features2d.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_core.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_contrib.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_calib3d.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_nonfree.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_gpu.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_photo.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_objdetect.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_legacy.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_video.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_ml.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_calib3d.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_features2d.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_highgui.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_imgproc.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_flann.so.2.4.9
samples/test_dbr_recoall_server.bin: /disk1/deeplearning/local_install/lib/libopencv_core.so.2.4.9
samples/test_dbr_recoall_server.bin: samples/CMakeFiles/test_dbr_recoall_server.bin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable test_dbr_recoall_server.bin"
	cd /home/app/project/test/cpp/alg/build/samples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_dbr_recoall_server.bin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
samples/CMakeFiles/test_dbr_recoall_server.bin.dir/build: samples/test_dbr_recoall_server.bin
.PHONY : samples/CMakeFiles/test_dbr_recoall_server.bin.dir/build

samples/CMakeFiles/test_dbr_recoall_server.bin.dir/requires: samples/CMakeFiles/test_dbr_recoall_server.bin.dir/dbr_recoall_server.cpp.o.requires
.PHONY : samples/CMakeFiles/test_dbr_recoall_server.bin.dir/requires

samples/CMakeFiles/test_dbr_recoall_server.bin.dir/clean:
	cd /home/app/project/test/cpp/alg/build/samples && $(CMAKE_COMMAND) -P CMakeFiles/test_dbr_recoall_server.bin.dir/cmake_clean.cmake
.PHONY : samples/CMakeFiles/test_dbr_recoall_server.bin.dir/clean

samples/CMakeFiles/test_dbr_recoall_server.bin.dir/depend:
	cd /home/app/project/test/cpp/alg/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/app/project/test/cpp/alg /home/app/project/test/cpp/alg/samples /home/app/project/test/cpp/alg/build /home/app/project/test/cpp/alg/build/samples /home/app/project/test/cpp/alg/build/samples/CMakeFiles/test_dbr_recoall_server.bin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : samples/CMakeFiles/test_dbr_recoall_server.bin.dir/depend

