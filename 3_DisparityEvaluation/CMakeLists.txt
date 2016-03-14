cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

project(DisparityEvaluation)

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

add_executable(DisparityEvaluation ${SOURCES})

target_link_libraries(DisparityEvaluation ${OpenCV_LIBS})
