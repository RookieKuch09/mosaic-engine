# Set CMake minimum version
cmake_minimum_required(VERSION 3.20)

# Create project
project(MosaicEngine LANGUAGES CXX)

# Configure C++ compiler
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Locate all sources
file(GLOB_RECURSE SOURCES "mosaic/source/**/*.cpp")

# Create the library
add_library(Mosaic)

# Set the library's sources
target_sources(Mosaic PRIVATE ${SOURCES})

# Find all required packages
find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED)

# Link library against required packages
target_link_libraries(Mosaic PRIVATE SDL3::SDL3)
target_link_libraries(Mosaic PRIVATE Vulkan::Vulkan)

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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mosaic/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mosaic/inline>
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
install(DIRECTORY mosaic/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mosaic/include)
install(DIRECTORY mosaic/inline/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mosaic/inline)

# Install primary interface files
install(DIRECTORY mosaic/interface/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Mosaic)

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