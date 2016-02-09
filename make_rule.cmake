add_executable(girl src/girl.cpp)
add_executable(girl-ls src/girl-ls.cpp)
add_library(location lib/location.cpp)
add_library(utils lib/utils.cpp)
install (TARGETS girl DESTINATION bin)

target_link_libraries(utils
  location
  ${Boost_SYSTEM_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES}
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)

target_link_libraries(girl
  location
  utils
  ${Boost_SYSTEM_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES}
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)

target_link_libraries(girl-ls
  location
  utils
  ${Boost_LIBRARIES}
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR})
