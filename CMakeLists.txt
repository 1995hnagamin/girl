cmake_minimum_required(VERSION 2.8)
project(girl)
add_subdirectory(lib)
add_executable(girl src/girl.cpp)
install (TARGETS girl DESTINATION bin)

set(Boost_USE_STATIC_LIBS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
find_package(Boost 1.58.0 COMPONENTS system filesystem program_options REQUIRED)
target_link_libraries(girl
  location
  ${Boost_SYSTEM_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES}
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)

include_directories (
  "${PROJECT_SOURCE_DIR}/lib"
  ${Boost_INCLUDE_DIRS}
)
