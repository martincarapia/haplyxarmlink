# Set the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(HaplyXArmLink)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories (adjust paths based on your project structure)
include_directories(
    ${PROJECT_SOURCE_DIR}/deps/macos/HardwareAPI.0.2.8.macos/include
    ${PROJECT_SOURCE_DIR}/deps/xArm-CPLUS-SDK/include
)

# Link directories (where CMake will look for libraries)
link_directories(
    ${PROJECT_SOURCE_DIR}/deps/macos/HardwareAPI.0.2.8.macos
    ${PROJECT_SOURCE_DIR}/deps/xArm-CPLUS-SDK/build/lib
)

# Add source files
set(SOURCES
    src/main.cpp
    # Add other .cpp files from your src directory here
)

# Add the executable
add_executable(main ${SOURCES})

# Link the libraries
target_link_libraries(main
    Haply.HardwareAPI
    xarm
    # Add other libraries you may need
)

# Custom command to build xArm-CPLUS-SDK
add_custom_target(
    xarm_sdk
    COMMAND make -C ${PROJECT_SOURCE_DIR}/deps/xArm-CPLUS-SDK xarm
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/xArm-CPLUS-SDK
    COMMENT "Building xArm-CPLUS-SDK"
)

# Ensure your project depends on xArm-CPLUS-SDK
add_dependencies(main xarm_sdk)

# Copy libxarm.so to the build/lib directory after building the target
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${PROJECT_SOURCE_DIR}/deps/xArm-CPLUS-SDK/build/lib/libxarm.so
    ${CMAKE_BINARY_DIR}/build/lib/libxarm.so
    COMMENT "Copying libxarm.so to build directory"
)

