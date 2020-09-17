set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(
  permissionerc
  src/permissionerc.cpp
)

target_link_libraries(
  permissionerc
  PUBLIC
  permissioner
)

