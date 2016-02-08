add_executable(girl src/girl.cpp)

install (TARGETS girl DESTINATION bin)

target_link_libraries(girl
  location
  ${Boost_SYSTEM_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES}
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)
