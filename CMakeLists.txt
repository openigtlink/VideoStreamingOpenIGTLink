cmake_minimum_required(VERSION 2.8)
project( VideoStreamingOpenIGTLink )

configure_file(CMakeListsOpenH264.txt.in
  OpenH264-download/CMakeLists.txt)
#Here the downloading project is triggered                                                               
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" . 
                WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/OpenH264-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build . 
                WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/OpenH264-download" )                   
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")             
        execute_process(COMMAND "make" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/OpenH264" )                     
endif(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows") 
            
configure_file(CMakeListsOpenIGTLink.txt.in
  OpenIGTLink-download/CMakeLists.txt)
#Here the downloading project is triggered                                                               
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" . 
                WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/OpenIGTLink-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build . 
                WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/OpenIGTLink-download" )                        
                
ADD_SUBDIRECTORY(VideoStreamServer)
ADD_SUBDIRECTORY(VideoStreamReceiver)
