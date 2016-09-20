cmake_minimum_required(VERSION 2.8)
project( VideoStreamingOpenIGTLink )

configure_file(CMakeListsOpenH264.txt.in
               OpenH264-download/CMakeLists.txt)

configure_file(CMakeListsOpenIGTLink.txt.in
               OpenIGTLink-download/CMakeLists.txt)

ADD_SUBDIRECTORY(VideoStreamServer)