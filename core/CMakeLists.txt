project(
  myproject_core
  VERSION 0.1.0
  DESCRIPTION "Example library"
  LANGUAGES CXX)

#
# Project Variables
#
include(GNUInstallDirs)

# Custom variables for builtins
set(PACKAGE_TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_NAMESPACE_NAME ${PROJECT_NAME})
set(PACKAGE_TARGET_ALIAS ${PACKAGE_NAMESPACE_NAME}::${PACKAGE_TARGET_NAME})

# Variables for all source code locations
set(PACKAGE_CMAKE_CONFIG_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(PACKAGE_CONFIGURE_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/configure_files)
set(PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PACKAGE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PACKAGE_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

# Installation directories for header files, cmake configuration modules and libraries
set(PACKAGE_INSTALL_CMAKE_CONFIG_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PACKAGE_TARGET_NAME})
set(PACKAGE_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set(PACKAGE_INSTALL_LIB_DIR ${CMAKE_INSTALL_LIBDIR})
set(PACKAGE_INSTALL_RUNTIME_DIR ${CMAKE_INSTALL_BINDIR})

# Generated install files
set(PACKAGE_CMAKE_TARGETS_EXPORT_NAME ${PACKAGE_TARGET_NAME}Targets)

# Template files and auto-generated files which will be installed to lib/cmake/myproject
set(PACKAGE_CMAKE_CONFIG_TEMPLATE_FILE ${PACKAGE_CONFIGURE_FILE_DIR}/Config.cmake.in)
set(PACKAGE_CMAKE_CONFIG_FILE ${PACKAGE_CMAKE_CONFIG_DIR}/${PACKAGE_TARGET_NAME}Config.cmake)
set(PACKAGE_CMAKE_VERSION_FILE
    ${PACKAGE_CMAKE_CONFIG_DIR}/${PACKAGE_TARGET_NAME}ConfigVersion.cmake)
set(PACKAGE_CMAKE_TARGETS_FILE ${PACKAGE_CMAKE_CONFIG_DIR}/${PACKAGE_TARGET_NAME}Targets.cmake)

#
# Output build information
#
message(STATUS "Building ${PROJECT_NAME} in [${CMAKE_BUILD_TYPE}] mode")

#
# Add tests if in Debug mode
#
if(myproject_ENABLE_DEVELOPER_MODE)
  message(STATUS "Tests active for ${PROJECT_NAME}")
  add_subdirectory(tests)
endif()

#
# Configuration files
#
configure_file(${PACKAGE_CONFIGURE_FILE_DIR}/config.hpp.in
               ${PACKAGE_INCLUDE_DIR}/${PACKAGE_TARGET_NAME}/config.hpp)

#
# Add private sources
#
set(PACKAGE_PRIVATE_SOURCES "${PACKAGE_SOURCE_DIR}/core.cpp")

#
# Add public headers
#
set(PACKAGE_PUBLIC_HEADERS "${PACKAGE_INCLUDE_DIR}/core.hpp" "${PACKAGE_INCLUDE_DIR}/config.hpp")

#
# Create the library
#
add_library(${PACKAGE_TARGET_NAME} STATIC)
add_library(${PACKAGE_TARGET_ALIAS} ALIAS ${PACKAGE_TARGET_NAME})

#
# Add private sources to the library
#
target_sources(${PACKAGE_TARGET_NAME} PRIVATE ${PACKAGE_PRIVATE_SOURCES})

#
# Declare public header files (will also be made available to IDE) and version the library
#
set_target_properties(
  ${PACKAGE_TARGET_NAME}
  PROPERTIES PUBLIC_HEADER "${PACKAGE_PUBLIC_HEADERS}"
             VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_VERSION_MAJOR})

# Link required dependencies
target_link_libraries(${PACKAGE_TARGET_NAME} PRIVATE myproject_workspace::myproject_options
                                                     myproject_workspace::myproject_warnings)

target_include_directories(${PACKAGE_TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${PACKAGE_INCLUDE_DIR}>
                                                         $<INSTALL_INTERFACE:include>)

if(myproject_ENABLE_DEVELOPER_MODE)
  run_clang_format(${PACKAGE_INCLUDE_DIR})
  run_clang_format(${PACKAGE_SOURCE_DIR})
  run_clang_format(${PACKAGE_TEST_DIR})
endif()

#
# Prepare for installation only if we aren't in developer mode
#
if(NOT myproject_ENABLE_DEVELOPER_MODE)
  include(CMakePackageConfigHelpers)

  # Strip out the INTERFACE_LINK_LIBRARIES for install (avoid dragging in myproject_options,
  # myproject_warnings on downstream targets)
  set_target_properties(${PACKAGE_TARGET_NAME} PROPERTIES INTERFACE_LINK_LIBRARIES "" EXPORT_ONLY
                                                                                      TRUE)

  # Strip out the PUBLIC_HEADER install target, this is only for IDE integration during development
  set_target_properties(${PACKAGE_TARGET_NAME} PROPERTIES PUBLIC_HEADER "")

  # Generate the CMake config and version files
  configure_package_config_file(
    "${PACKAGE_CMAKE_CONFIG_TEMPLATE_FILE}" "${PACKAGE_CMAKE_CONFIG_FILE}"
    INSTALL_DESTINATION "${PACKAGE_INSTALL_CMAKE_CONFIG_DIR}")

  write_basic_package_version_file(
    "${PACKAGE_CMAKE_VERSION_FILE}"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

  # Install public header files
  install(DIRECTORY "${PACKAGE_INCLUDE_DIR}/" DESTINATION ${PACKAGE_INSTALL_INCLUDE_DIR})

  # Install the library, its headers, and export the targets
  install(
    TARGETS ${PACKAGE_TARGET_NAME}
    EXPORT ${PACKAGE_CMAKE_TARGETS_EXPORT_NAME}
    ARCHIVE DESTINATION ${PACKAGE_INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${PACKAGE_INSTALL_LIB_DIR}
    RUNTIME DESTINATION ${PACKAGE_INSTALL_RUNTIME_DIR})

  # Install the exported CMake “myprojectTargets.cmake” file
  install(
    EXPORT ${PACKAGE_CMAKE_TARGETS_EXPORT_NAME}
    FILE ${PACKAGE_CMAKE_TARGETS_EXPORT_NAME}.cmake
    NAMESPACE ${PACKAGE_NAMESPACE_NAME}::
    DESTINATION ${PACKAGE_INSTALL_CMAKE_CONFIG_DIR})

  # Install the package‐config and version files
  install(FILES "${PACKAGE_CMAKE_CONFIG_FILE}" "${PACKAGE_CMAKE_VERSION_FILE}"
          DESTINATION ${PACKAGE_INSTALL_CMAKE_CONFIG_DIR})

endif()
