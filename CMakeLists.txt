cmake_minimum_required(VERSION 2.8.8)
project(NosC)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++11")

# We want the executables separate
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(idlgen)
