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
  ./testConfig
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cfg
)
