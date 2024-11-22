cmake_minimum_required (VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(SrcFiles
	${MY_SRC_DIR}/indexfile.cpp
	${MY_SRC_DIR}/request.cpp
	${MY_SRC_DIR}/config.cpp
	${MY_SRC_DIR}/lib.cpp
)

set(TestsFiles
	indexfile_test.cpp
	request_test.cpp
	config_test.cpp
)

add_executable(Gtest ${TestsFiles} ${SrcFiles})
target_link_libraries(Gtest nlohmann GTest::gtest_main)

include(GoogleTest)
target_include_directories(Gtest PUBLIC ${MY_INCLUDE_DIR} ${GTEST_INCLUDE_DIRS})
gtest_discover_tests(Gtest)
