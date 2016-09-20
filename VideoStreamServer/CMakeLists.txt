cmake_minimum_required(VERSION 2.8)
project( VideoStreamServer )

find_package(OpenIGTLink REQUIRED)
include(${OpenIGTLink_USE_FILE})
include_directories(${OpenIGTLink_INCLUDE_DIRS})
link_directories(${OpenIGTLink_LIBRARY_DIRS})
include_directories(${PROJECT_BINARY_DIR})

add_executable( VideoStreamServer VideoStreamServer.cpp )
target_link_libraries( VideoStreamServer OpenIGTLink)
