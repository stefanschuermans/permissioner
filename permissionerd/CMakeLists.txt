add_executable(
  permissionerd
  src/permissionerd.cpp
)

target_link_libraries(
  permissionerd
  PUBLIC
  permissioner
)
