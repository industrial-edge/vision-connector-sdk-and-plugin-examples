cmake_minimum_required(VERSION 3.21)

include(CTest)
include(GoogleTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fno-inline -fno-inline-small-functions -fno-default-inline")
set(TEST_EXECUTABLE_NAME "${LIBRARY_NAME}-Test")

file(GLOB_RECURSE TEST_SOURCES
	*.cpp
)
list(APPEND TEST_SOURCES "${PROJECT_SOURCES}")

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

set(LIBS VCA-SDK ${LIBS})
separate_arguments(LIBLIST UNIX_COMMAND "${ADDTIONAL_LIBS}")
foreach(LIB ${LIBLIST})
   message(STATUS "len = ${LIB}")
   set(LIBS ${LIB} ${LIBS})
endforeach()

add_executable(${TEST_EXECUTABLE_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_EXECUTABLE_NAME} VCA-SDK ${GTEST_LIBRARIES} ${OpenCV_LIBS} Threads::Threads ${LIBS})
target_compile_definitions(${TEST_EXECUTABLE_NAME} PRIVATE TESTING)

if(EXISTS ${PROJECT_SOURCE_DIR}/lib)
    add_custom_command(
        TARGET ${TEST_EXECUTABLE_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/lib/*
            ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()

add_custom_command(
    TARGET ${TEST_EXECUTABLE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${SDK_LIBRARY_DIR}/libVCA-SDK.so
        ${CMAKE_CURRENT_BINARY_DIR}
)

gtest_discover_tests(${TEST_EXECUTABLE_NAME})