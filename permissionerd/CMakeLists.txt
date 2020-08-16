add_executable(
  permissionerd
  src/Config.cpp
  src/Config.h
  src/permissionerd.cpp
  src/Permissions.cpp
  src/Permissions.h
  src/StringUtils.cpp
  src/StringUtils.h
  src/Tree.cpp
  src/Tree.h
)

target_link_libraries(
  permissionerd
  PUBLIC
  Boost::filesystem
)
