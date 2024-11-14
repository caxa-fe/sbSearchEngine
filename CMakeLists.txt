cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)

set(PATCH_VERSION "0" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.1.${PATCH_VERSION})

project(sbSearchEngine VERSION ${PROJECT_VERSION} LANGUAGES C CXX)

configure_file(lib/constants.h.in ${CMAKE_CURRENT_SOURCE_DIR}/lib/constants.h )

set(SOURCE_EXE main.cpp)	
set(SOURCE_LIB lib/lib.cpp)	
add_library(lib STATIC ${SOURCE_LIB})

add_library(nlohmann INTERFACE)
target_include_directories(nlohmann INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann/)

include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann/
)
add_library(config lib/config.cpp)
add_library(indexfile lib/indexfile.cpp)
add_library(request lib/request.cpp)

add_executable(sbSearchEngine main.cpp)
target_link_libraries(sbSearchEngine lib nlohmann config indexfile request)

## ---- Tests -----

enable_testing()
cmake_policy(SET CMP0144 NEW)

set(GTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/google)
find_package(GTest REQUIRED)

if(GTest_FOUND) 
  message("--- Google test found")
  # message(${GTEST_INCLUDE_DIRS})
  # message(${GTEST_ROOT})
  # message(${GTEST_BOTH_LIBRARIES})
  # message(${GTEST_LIBRARIES})
  # message(${GTEST_MAIN_LIBRARIES})
endif()
#find_library(GTest gtest)
add_executable(sbtest ${CMAKE_CURRENT_SOURCE_DIR}/tests/sbtest.cpp)
target_include_directories(sbtest PRIVATE ${GTEST_INCLUDE_DIRS})
#target_link_libraries(sbtest PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(sbtest PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

