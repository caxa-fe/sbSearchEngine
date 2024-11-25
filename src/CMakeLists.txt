cmake_minimum_required (VERSION 3.24)

set(Src
	"config.cpp"
	"indexfile.cpp"
	"lib.cpp"
	"request.cpp"
	"functions.cpp"
	"main.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(sbSearchEngine ${Src})

add_library(nlohmann INTERFACE)
target_include_directories(nlohmann INTERFACE ${MY_INCLUDE_DIR}/nlohmann/)

target_link_libraries(sbSearchEngine nlohmann)
target_include_directories(sbSearchEngine PUBLIC ${MY_INCLUDE_DIR})