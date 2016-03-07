cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

project(DisparityAlgorithm)

find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS system filesystem program_options REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp)

set(INCLUDE_DIRS "")
foreach(_headerFile ${HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list(APPEND INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)
include_directories(${INCLUDE_DIRS})

add_executable(DisparityAlgorithm ${SOURCES})

target_link_libraries(DisparityAlgorithm ${OpenCV_LIBS})
target_link_libraries(DisparityAlgorithm ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
