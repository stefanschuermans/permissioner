add_library(
  permissioner
  STATIC
  include/permissioner/Config.h
  include/permissioner/Permissions.h
  include/permissioner/StringUtils.h
  include/permissioner/Tree.h
  src/Config.cpp
  src/Permissions.cpp
  src/StringUtils.cpp
  src/Tree.cpp
)

target_include_directories(
  permissioner
  PUBLIC
  include
)

target_link_libraries(
  permissioner
  PUBLIC
  Boost::filesystem
)
