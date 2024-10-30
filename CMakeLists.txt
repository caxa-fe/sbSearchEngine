cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(sbSearchEngine VERSION ${PROJECT_VERSION} LANGUAGES C CXX)

configure_file(lib/constants.h.in ${CMAKE_CURRENT_SOURCE_DIR}/lib/constants.h )

set(SOURCE_EXE main.cpp)	
set(SOURCE_LIB lib/lib.cpp)	
add_library(lib STATIC ${SOURCE_LIB})

add_library(nlohmann INTERFACE)
target_include_directories(nlohmann INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann/)

add_library(converterJSON STATIC lib/converterJSON.cpp)
add_library(textdoc STATIC lib/textdoc.cpp)

add_library(searchserver STATIC lib/searchserver.cpp)


add_library(invertedindex STATIC lib/invertedindex.cpp)
add_library(request STATIC lib/request.cpp)

include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann/
)


add_executable(sbSearchEngine ${SOURCE_EXE})

target_link_libraries(sbSearchEngine lib nlohmann converterJSON textdoc invertedindex request searchserver)	

## ---- Tests -----

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 2.24:
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/df1544bcee0c7ce35cd5ea0b3eb8cc81855a4140.zip
)
# --- This added by Skillbox
include_directories(${gtest_SOURCE_DIR}/test_resources ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
# --- End Skillbox

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_EXE tests/sbtest.cpp)
add_executable(  sbtest  ${TEST_EXE} )

target_link_libraries(sbtest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(sbtest)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
