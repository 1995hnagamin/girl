add_executable(girl src/girl.cpp)
add_executable(girl-ls src/girl-ls.cpp)

install (TARGETS girl DESTINATION bin)

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
