cmake_minimum_required(VERSION 3.27)
project(Sunlight VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE SOURCES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
add_subdirectory(ext/Lumiere)
target_link_libraries(Sunlight PRIVATE Lumiere)