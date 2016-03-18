cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
project(DisparityAlgorithm)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp)

set(INCLUDE_DIRS "")
foreach (_headerFile ${HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list(APPEND INCLUDE_DIRS ${_dir})
endforeach ()
list(REMOVE_DUPLICATES INCLUDE_DIRS)
include_directories(${INCLUDE_DIRS})

add_executable(DisparityAlgorithm ${SOURCES})
target_link_libraries(DisparityAlgorithm ${OpenCV_LIBS})
