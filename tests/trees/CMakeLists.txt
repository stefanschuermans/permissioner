add_executable(
  testTrees
  testTrees.cpp
)

target_link_libraries(
  testTrees
  PUBLIC
  permissioner
)

add_test(
  NAME
  testTrees
  COMMAND
  ${CMAKE_CURRENT_SOURCE_DIR}/testTrees.bash
)
