cmake_minimum_required(VERSION 2.8)
project( VideoStreamReceiver )

set(CMAKE_PREFIX_PATH	"${CMAKE_BINARY_DIR}/OpenIGTLink-bin")	
find_package(OpenIGTLink REQUIRED)
include(${OpenIGTLink_USE_FILE})
include_directories(${OpenIGTLink_INCLUDE_DIRS})
link_directories(${OpenIGTLink_LIBRARY_DIRS})
#Add OpenH264 directly to our build.
#add_subdirectory(${CMAKE_BINARY_DIR}/Testing/OpenH264)
include_directories("${CMAKE_BINARY_DIR}/OpenH264/codec")
include_directories("${CMAKE_BINARY_DIR}/OpenH264/test")

LINK_DIRECTORIES("${CMAKE_BINARY_DIR}/OpenH264")

add_executable( VideoStreamReceiver VideoStreamReceiver.cxx)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_link_libraries( VideoStreamReceiver OpenIGTLink ${CMAKE_BINARY_DIR}/OpenH264/OpenH264.lib)
else(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_link_libraries( VideoStreamReceiver OpenIGTLink ${CMAKE_BINARY_DIR}/OpenH264/libOpenH264.a)
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
