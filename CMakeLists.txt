cmake_minimum_required(VERSION 3.10)

set(BADGECPP_MAJOR_VERSION "0")
set(BADGECPP_MINOR_VERSION "0")
set(BADGECPP_PATCH_VERSION "1")
set(BADGECPP_VERSION_SPECIAL "")
set(BADGECPP_VERSION_STRING "${BADGECPP_MAJOR_VERSION}.${BADGECPP_MINOR_VERSION}.${BADGECPP_PATCH_VERSION}")

if(NOT "${BADGECPP_VERSION_SPECIAL}" STREQUAL "")
    set(BADGECPP_VERSION_STRING "${BADGECPP_VERSION_STRING}-${BADGECPP_VERSION_SPECIAL}")
endif()

project(badgecpp VERSION ${BADGECPP_VERSION_STRING} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "src/**")
add_library(badgecpp STATIC ${SOURCES})
target_include_directories(badgecpp PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_compile_definitions(badgecpp PUBLIC "BADGECPP_VERSION_MAJOR=${BADGECPP_MAJOR_VERSION}")
target_compile_definitions(badgecpp PUBLIC "BADGECPP_VERSION_MINOR=${BADGECPP_MINOR_VERSION}")
target_compile_definitions(badgecpp PUBLIC "BADGECPP_VERSION_PATCH=${BADGECPP_PATCH_VERSION}")
target_compile_definitions(badgecpp PUBLIC "BADGECPP_VERSION_SPECIAL=\"${BADGECPP_VERSION_SPECIAL}\"")
target_compile_definitions(badgecpp PUBLIC "BADGECPP_VERSION_STRING=\"${BADGECPP_VERSION_STRING}\"")

if(MSVC)
    target_compile_options(badgecpp PRIVATE /W4 /WX
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Release>:/O2>)
else()
    target_compile_options(badgecpp PRIVATE -Wall -Werror
        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:Release>:-O3>)
endif()

option(BADGECPP_TESTS "Build the badgecpp test executable" OFF)

if(BADGECPP_TESTS)
    add_executable(badgecpp_test "main.cpp")
    target_include_directories(badgecpp_test PRIVATE "${PROJECT_SOURCE_DIR}/inc")

    target_compile_options(badgecpp_test PRIVATE
        $<$<CONFIG:Debug>:-O0 -g>
    )

    enable_testing()
    add_test(NAME BadgecppTest COMMAND badgecpp_test)
    message(STATUS "Enabled badgecpp tests")
endif()
