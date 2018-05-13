cmake_minimum_required(VERSION 3.11)

set(MY_TARGET_NAME "e131bridge")

project(${MY_TARGET_NAME})

file(GLOB SOURCES "src/*.cpp")

link_directories("lib/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -DBOOST_LOG_DYN_LINK")

find_package(Boost 1.66.0 COMPONENTS log program_options REQUIRED)
find_library(E131 e131)
find_library(YAML_CPP_LIBRARY yaml-cpp)
find_library(BCM bcm2835)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(${MY_TARGET_NAME} ${SOURCES})

target_link_libraries(${MY_TARGET_NAME} ${YAML_CPP_LIBRARY} ${E131} ${BCM})
#target_link_libraries(${MY_TARGET_NAME} ${CMAKE_SOURCE_DIR}/lib/libws2811.a)
target_link_libraries(${MY_TARGET_NAME} ${Boost_LIBRARIES})
