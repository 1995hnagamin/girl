cmake_minimum_required(VERSION 2.8)
project(girl)

set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
find_package(Boost 1.58.0 COMPONENTS system filesystem)
if (Boost_FOUND)
  add_executable(girl src/girl.cpp)
  target_link_libraries(girl ${Boost_LIBRARIES})
endif()
