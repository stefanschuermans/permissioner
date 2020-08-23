add_executable(
  testConfig
  testConfig.cpp
)

target_link_libraries(
  testConfig
  PUBLIC
  permissioner
)

add_test(
  NAME
  testConfig
  COMMAND
  ${CMAKE_CURRENT_BINARY_DIR}/testConfig test.cfg
  WORKING_DIRECTORY
  ${CMAKE_CURRENT_SOURCE_DIR}
)
