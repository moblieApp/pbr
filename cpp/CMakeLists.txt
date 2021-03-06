#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.
#

cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fPIC -std=c++11")

#SET(Boost_ADDITIONAL_VERSIONS "1.58" "1.58.0")
#SET(BOOST_ROOT "/usr/local/include/boost/")
set(Boost_INCLUDE_DIR /usr/local/include)
set(Boost_LIBRARY_DIR /usr/local/lib)

find_package(OpenCV REQUIRED)

#FIND_PACKAGE (Boost REQUIRED) 
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost REQUIRED)
include_directories(SYSTEM "${Boost_INCLUDE_DIRS}")

#Make sure gen-cpp files can be included
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/gen-cpp")
include_directories("${PROJECT_SOURCE_DIR}/lib")

#include(ThriftMacros)
message(${CMAKE_CURRENT_BINARY_DIR})
message(${PROJECT_SOURCE_DIR})

include_directories("${CMAKE_CURRENT_BINARY_DIR}/alg/include")
link_directories("${PROJECT_SOURCE_DIR}/alg/lib")

set(utilscpp_SOURCES
    utils/base64.cpp
)
add_library(utilscpp STATIC ${utilscpp_SOURCES})

set(tutorialgencpp_SOURCES
    gen-cpp/Calculator.cpp
    gen-cpp/SharedService.cpp
    gen-cpp/shared_constants.cpp
    gen-cpp/shared_types.cpp
    gen-cpp/tutorial_constants.cpp
    gen-cpp/tutorial_types.cpp
)
add_library(tutorialgencpp STATIC ${tutorialgencpp_SOURCES})
target_link_libraries(tutorialgencpp thrift)
#LINK_AGAINST_THRIFT_LIBRARY(tutorialgencpp thrift)

#message(${ZLIB_LIBRARIES})
#add_custom_command(OUTPUT gen-cpp/Calculator.cpp gen-cpp/SharedService.cpp gen-cpp/shared_constants.cpp gen-cpp/shared_types.cpp gen-cpp/tutorial_constants.cpp gen-cpp/tutorial_types.cpp
#    COMMAND ${THRIFT_COMPILER} --gen cpp -r ${PROJECT_SOURCE_DIR}/tutorial/tutorial.thrift
#)

add_executable(TutorialServer CppServer.cpp)
target_link_libraries(TutorialServer tutorialgencpp)
target_link_libraries(TutorialServer utilscpp)
TARGET_LINK_LIBRARIES(TutorialServer sn_drawbook)
#LINK_AGAINST_THRIFT_LIBRARY(TutorialServer thrift)
target_link_libraries(TutorialServer thrift)
target_link_libraries(TutorialServer ${ZLIB_LIBRARIES})
TARGET_LINK_LIBRARIES(TutorialServer ${OpenCV_LIBS})

add_executable(TutorialClient CppClient.cpp)
target_link_libraries(TutorialClient tutorialgencpp)
target_link_libraries(TutorialServer thrift)
#LINK_AGAINST_THRIFT_LIBRARY(TutorialClient thrift)
target_link_libraries(TutorialClient ${ZLIB_LIBRARIES})
