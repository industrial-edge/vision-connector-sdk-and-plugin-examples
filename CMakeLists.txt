cmake_minimum_required(VERSION 3.21)

project(VCA-Drivers)

enable_testing()

file(CREATE_LINK
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json"
  SYMBOLIC
)

option(TARGET_DRIVER "Driver target to configure")
if(TARGET_DRIVER STREQUAL DAHENG)
	add_subdirectory(drivers/daheng)
endif()
