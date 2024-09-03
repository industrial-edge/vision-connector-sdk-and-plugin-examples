cmake_minimum_required(VERSION 3.21)

set(LIBRARY_NAME "allied")

project(${LIBRARY_NAME})
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-fcf-protection=full -Wall -Wextra -Wsign-promo -Wnon-virtual-dtor -Wreorder -Wformat=2 -Wformat-security -Wcast-align -Wstrict-overflow -Wtrampolines ${CMAKE_CXX_FLAGS}")

set(CMAKE_SKIP_BUILD_INSTALL_RPATH OFF)
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(CMAKE_INSTALL_RPATH $ORIGIN)

file(GLOB_RECURSE PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src/*)

include_directories(${SDK_INCLUDE_DIR})
link_directories(${SDK_LIBRARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

if(TESTING STREQUAL "ON")
    add_subdirectory(tests)
endif()

add_library(${LIBRARY_NAME} SHARED ${PROJECT_SOURCES})
target_link_libraries(${LIBRARY_NAME} VCA-SDK gxiapi)
target_compile_options(${LIBRARY_NAME} PRIVATE -fno-gnu-unique)

install(TARGETS ${LIBRARY_NAME} DESTINATION ${LIBRARY_NAME})
install(DIRECTORY ${SDK_LIBRARY_DIR}/
        DESTINATION ${LIBRARY_NAME}
        FILES_MATCHING
            PATTERN "libVCA-SDK.so"
            PATTERN "info.txt"
            PATTERN "include" EXCLUDE
)

if(EXISTS ${PROJECT_SOURCE_DIR}/lib)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/lib/
        DESTINATION ${LIBRARY_NAME}
        FILES_MATCHING PATTERN "*"
    )
endif()