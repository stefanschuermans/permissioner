add_executable(
  testConfig
  testConfig.cpp
  ${CMAKE_SOURCE_DIR}/permissionerd/src/Config.cpp
  ${CMAKE_SOURCE_DIR}/permissionerd/src/Permissions.cpp
  ${CMAKE_SOURCE_DIR}/permissionerd/src/StringUtils.cpp
  ${CMAKE_SOURCE_DIR}/permissionerd/src/Tree.cpp
)

target_include_directories(
  testConfig
  PUBLIC
  ${CMAKE_SOURCE_DIR}/permissionerd/src
)

target_link_libraries(
  testConfig
  PUBLIC
  Boost::filesystem
)

add_test(
  NAME
  testConfig
  COMMAND
  ./testConfig
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cfg
)
