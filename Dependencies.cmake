include(cmake/CPM.cmake)

# Done as a function so that updates to variables like CMAKE_CXX_FLAGS don't
# propagate out to other targets
function(myproject_setup_dependencies)

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(NOT TARGET gtest)
      cpmaddpackage(
        NAME
        googletest
        GITHUB_REPOSITORY
        google/googletest
        GIT_TAG
        v1.15.2
        VERSION
        1.15.2
        OPTIONS
        "BUILD_GMOCK ON"
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON")
    endif()
  endif()
endfunction()
