cmake_minimum_required (VERSION 3.5)

project (lane_finding)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")

file (GLOB source_files "${source_dir}/*.cpp" "${source_dir}/*.h")

add_executable (lane_finding ${source_files})

target_link_libraries(lane_finding ${OpenCV_LIBS})
