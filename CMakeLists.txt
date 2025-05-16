cmake_minimum_required(VERSION 3.10)
project(GarbageCollector VERSION 1.0.0 LANGUAGES C)

# Options
option(BUILD_EXAMPLES "Build example applications" ON)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Library sources
set(SOURCES
    src/gc.c
)

# Library headers
set(HEADERS
    include/gc.h
)

# Create library target
add_library(gc ${SOURCES})
target_include_directories(gc PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Examples
if(BUILD_EXAMPLES)
    add_executable(example examples/example.c)
    target_link_libraries(example gc)
    target_include_directories(example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Installation
install(TARGETS gc
    EXPORT GarbageCollectorTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES ${HEADERS} DESTINATION include)

# Export targets
install(EXPORT GarbageCollectorTargets
    FILE GarbageCollectorTargets.cmake
    NAMESPACE GarbageCollector::
    DESTINATION lib/cmake/GarbageCollector
)

# Create and install config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/GarbageCollectorConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/GarbageCollectorConfig.cmake
    INSTALL_DESTINATION lib/cmake/GarbageCollector
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GarbageCollectorConfig.cmake
    DESTINATION lib/cmake/GarbageCollector
)