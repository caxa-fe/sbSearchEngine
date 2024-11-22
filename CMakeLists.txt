cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)

set(PATCH_VERSION "0" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.1.${PATCH_VERSION})

project(sbSearchEngine VERSION ${PROJECT_VERSION} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

set(MY_INCLUDE_DIR
  ${CMAKE_SOURCE_DIR}/include
)
set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

configure_file(constants.h.in ${MY_INCLUDE_DIR}/constants.h )

#====== TESTS =========

cmake_policy(SET CMP0144 NEW)
set(GTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/tests/google)
find_package(GTest REQUIRED)

if(GTest_FOUND) 
  message("--- Google test found ---")
  message(${GTEST_INCLUDE_DIRS})
  message(${GTEST_ROOT})
  #message(${GTEST_BOTH_LIBRARIES})
  message(${GTEST_LIBRARIES})
  message(${GTEST_MAIN_LIBRARIES})
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
