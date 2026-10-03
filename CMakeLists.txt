cmake_minimum_required(VERSION 3.16)
project(BootScope
    VERSION 1.0.0
    DESCRIPTION "Linux Boot-Time Profiler & Dependency Analyzer"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for strict diagnostics and optimization
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror=return-type
    -O2
)

# Include directories
include_directories(include)

# Core library source files
set(BOOTSCOPE_LIB_SOURCES
    src/boot_collector.cpp
    src/service_analyzer.cpp
    src/dependency_graph.cpp
    src/bottleneck_detector.cpp
    src/report_generator.cpp
)

# Build static library for modular reuse and unit testing
add_library(bootscope_core STATIC ${BOOTSCOPE_LIB_SOURCES})

# Build main CLI executable
add_executable(bootscope src/main.cpp)
target_link_libraries(bootscope PRIVATE bootscope_core)

# Build unit tests
enable_testing()
add_executable(bootscope_tests tests/test_bootscope.cpp)
target_link_libraries(bootscope_tests PRIVATE bootscope_core)
add_test(NAME AllTests COMMAND bootscope_tests)

# Installation target
install(TARGETS bootscope DESTINATION bin)
install(DIRECTORY config/ DESTINATION /etc/bootscope OPTIONAL)
