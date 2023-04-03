cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME GameEngineECS)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

project(${PROJECT_NAME})

file(GLOB source_files 
    src/*.cpp
    src/imgui/*.cpp
)

find_package(SFML 2 REQUIRED COMPONENTS audio graphics window system)

include_directories("include/imgui-sfml")
include_directories("include/imgui")


add_executable(${PROJECT_NAME} ${source_files})

target_include_directories(${PROJECT_NAME} PRIVATE "include")
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio GL)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})