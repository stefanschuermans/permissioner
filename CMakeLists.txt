cmake_minimum_required(VERSION 3.10)
project(permissioner)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

find_package(
  Boost
  COMPONENTS
  filesystem
)

enable_testing()

add_custom_command(
  OUTPUT
  ${CMAKE_CURRENT_BINARY_DIR}/exports
  DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/exports
  COMMAND
  cp -a ${CMAKE_CURRENT_SOURCE_DIR}/exports ${CMAKE_CURRENT_BINARY_DIR}/exports
)

add_custom_target(
  copy_exports
  ALL
  DEPENDS
  exports
)

add_subdirectory(libpermissioner)
add_subdirectory(permissionerd)
add_subdirectory(tests)
