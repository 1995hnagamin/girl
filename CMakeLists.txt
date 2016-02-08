cmake_minimum_required(VERSION 2.8)
project(girl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}
)
find_package(Boost 1.46.0 COMPONENTS system filesystem program_options REQUIRED)

message("Boost include dir:               " ${Boost_INCLUDE_DIR})
message("Boost system libraries:          " ${Boost_SYSTEM_LIBRARIES})
message("Boost filesystem libraries:      " ${Boost_FILESYSTEM_LIBRARIES})
message("Boost program options libraries: " ${Boost_PROGRAM_OPTIONS_LIBRARIES})

include_directories (
  "${PROJECT_SOURCE_DIR}/lib"
  ${Boost_INCLUDE_DIRS}
)

include(make_rule)
