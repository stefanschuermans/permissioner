set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(
  permissionerd
  src/permissionerd.cpp
)

target_link_libraries(
  permissionerd
  PUBLIC
  permissioner
)
