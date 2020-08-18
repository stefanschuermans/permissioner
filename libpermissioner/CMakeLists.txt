add_library(
  permissioner
  STATIC
  include/permissioner/Config.h
  include/permissioner/Group.h
  include/permissioner/Permissions.h
  include/permissioner/StringUtils.h
  include/permissioner/Tree.h
  include/permissioner/User.h
  src/Config.cpp
  src/Group.cpp
  src/Permissions.cpp
  src/StringUtils.cpp
  src/Tree.cpp
  src/User.cpp
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
