cmake_minimum_required(VERSION 3.21)

project(VCA-Drivers)

enable_testing()

file(CREATE_LINK
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json"
  SYMBOLIC
)

option(TARGET_DRIVER "Driver target to configure")
if(TARGET_DRIVER STREQUAL NUMBERS)
	add_subdirectory(drivers/numbers)
endif()

if(TARGET_DRIVER STREQUAL GLOBBING)
	add_subdirectory(drivers/globbing)
endif()

if(TARGET_DRIVER STREQUAL DAHENG)
	add_subdirectory(drivers/daheng)
endif()

if(TARGET_DRIVER STREQUAL BASLERLL)
	add_subdirectory(drivers/baslerll)
endif()

if(TARGET_DRIVER STREQUAL BASLER)
	add_subdirectory(drivers/basler)
endif()

if(TARGET_DRIVER STREQUAL ALLIED)
	add_subdirectory(drivers/allied)
endif()

if(TARGET_DRIVER STREQUAL NUMBERSWITHFILELOGGER)
	add_subdirectory(drivers/numberswithfilelogger)
endif()
