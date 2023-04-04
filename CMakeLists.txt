cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME GameEngineECS)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
set(OpenGL_GL_PREFERENCE "GLVND")

project(${PROJECT_NAME})

file(GLOB source_files 
    src/*.cpp
    src/imgui/*.cpp
)

find_package(SFML 2 REQUIRED COMPONENTS audio graphics window system)
find_package(OpenGL REQUIRED)

include_directories("include/imgui-sfml")
include_directories("include/imgui")
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${source_files})

target_include_directories(${PROJECT_NAME} PRIVATE "include")
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(${PROJECT_NAME} GL)
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})