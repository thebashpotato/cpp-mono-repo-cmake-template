project(
  myproject_app
  VERSION 0.1.0
  DESCRIPTION "Example executable application"
  LANGUAGES CXX)

#
# Set our source and include directory variables for use through the build
#
set(PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PACKAGE_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

#
# Add all source files
#
set(SOURCE_FILES "${PACKAGE_SOURCE_DIR}/main.cpp")

#
# Output build information
#
message(STATUS "Building ${PROJECT_NAME} in [${CMAKE_BUILD_TYPE}] mode")

if(myproject_ENABLE_DEVELOPER_MODE)
  message(STATUS "Tests active for ${PROJECT_NAME}")
  add_subdirectory(tests)
endif()

#
# Declare the executable and add all source modules
#
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE myproject_options myproject_warnings myproject_core)

if(myproject_ENABLE_DEVELOPER_MODE)
  run_clang_format(${PACKAGE_SOURCE_DIR})
  run_clang_format(${PACKAGE_TEST_DIR})
endif()

#
# Install the application
#
if(NOT myproject_ENABLE_DEVELOPER_MODE)
  include(GNUInstallDirs)

  # Install the built executable into <prefix>/bin (portable across platforms)
  install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # e.g. bin/
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR} # macOS app bundles
           COMPONENT runtime)

  # (Optional) If your app depends on shared libs you install to the same prefix, set an INSTALL_RPATH so the loader can
  # find them after install. Remove this if you don't ship any shared libs alongside the app.
  # if(APPLE)
  #   set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}")
  # elseif(UNIX)
  #   set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
  # endif()

  # (Optional) Extra assets / docs / completions / man page install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets/
  # DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME} COMPONENT runtime)
  #
  # install(FILES LICENSE README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
  #
  # install(FILES docs/${PROJECT_NAME}.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
  #
  # install(FILES packaging/completions/${PROJECT_NAME}.bash DESTINATION
  # ${CMAKE_INSTALL_DATAROOTDIR}/bash-completion/completions RENAME ${PROJECT_NAME})

  # (Optional) Minimal packaging with CPack set(CPACK_PACKAGE_NAME "${PROJECT_NAME}") set(CPACK_PACKAGE_VERSION
  # "${PROJECT_VERSION}") set(CPACK_PACKAGE_CONTACT "Your Name <you@example.com>") set(CPACK_RESOURCE_FILE_LICENSE
  # "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE") include(CPack)
endif()
