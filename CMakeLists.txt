# Set CMake minimum version
cmake_minimum_required(VERSION 3.20)

# Create project
project(MosaicEngine LANGUAGES CXX)

# Set export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Configure linker
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld")

# Configure C++ compiler
set(CMAKE_CXX_COMPILER "gcc")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

# Locate all sources
file(GLOB_RECURSE SOURCES "source/**/*.cpp")

# Create the library
add_library(Mosaic)

# Set the library's sources
target_sources(Mosaic PRIVATE ${SOURCES})

# Set include directories for the library
target_include_directories(Mosaic PRIVATE "include" "inline")

# Find all required packages
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# Link library against required packages
target_link_libraries(Mosaic PRIVATE SDL2::SDL2)
target_link_libraries(Mosaic PRIVATE GLEW::GLEW)
target_link_libraries(Mosaic PRIVATE OpenGL::GL)

# Clear install directory to avoid stale files
install(CODE "
    message(STATUS \"Clearing old Mosaic install directory...\")
    file(REMOVE_RECURSE \"\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/Mosaic\")
")

# Inclure required modules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Set the public include directory
target_include_directories(Mosaic
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inline>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mosaic/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Mosaic/inline>
)

# Install the library
install(TARGETS Mosaic
    EXPORT MosaicTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install headers and inline files
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mosaic/include)
install(DIRECTORY inline/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mosaic/inline)

# Install primary interface files
install(DIRECTORY interface/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mosaic)

# Export the CMake config for consumers
install(EXPORT MosaicTargets
    FILE MosaicTargets.cmake
    NAMESPACE Mosaic::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Mosaic
)

# Write package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfigVersion.cmake"
    VERSION 0.0.1
    COMPATIBILITY AnyNewerVersion
)

# Configure package version file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/MosaicConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Mosaic
)

# Install config files for package
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MosaicConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Mosaic
)

# Select Debug or Release compilation
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
else()
    add_compile_definitions(RELEASE)
endif()