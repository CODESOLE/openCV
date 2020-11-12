cmake_minimum_required(VERSION 3.15)
project(opencvTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(opencvTest ocv.cc)
target_link_libraries(opencvTest ${OpenCV_LIBS})
