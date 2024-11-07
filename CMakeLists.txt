cmake_minimum_required(VERSION 3.10)

set(BADGECPP_MAJOR_VERSION "0")
set(BADGECPP_MINOR_VERSION "0")
set(BADGECPP_PATCH_VERSION "1")
set(BADGECPP_VERSION_SPECIAL "")
set(BADGECPP_VERSION_STRING "${BADGECPP_MAJOR_VERSION}.${BADGECPP_MINOR_VERSION}.${BADGECPP_PATCH_VERSION}")

if(NOT "${BADGECPP_VERSION_SPECIAL}" STREQUAL "")
    set(BADGECPP_VERSION_STRING "${BADGECPP_VERSION_STRING}-${BADGECPP_VERSION_SPECIAL}")
endif()

project(badgecpp VERSION ${BADGECPP_VERSION_STRING} DESCRIPTION "Badge++ library" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "src/**")
add_library(badgecpp SHARED ${SOURCES})
target_include_directories(badgecpp PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_compile_definitions(badgecpp PRIVATE "BADGECPP_VERSION_MAJOR=${BADGECPP_MAJOR_VERSION}")
target_compile_definitions(badgecpp PRIVATE "BADGECPP_VERSION_MINOR=${BADGECPP_MINOR_VERSION}")
target_compile_definitions(badgecpp PRIVATE "BADGECPP_VERSION_PATCH=${BADGECPP_PATCH_VERSION}")
target_compile_definitions(badgecpp PRIVATE "BADGECPP_VERSION_SPECIAL=\"${BADGECPP_VERSION_SPECIAL}\"")
target_compile_definitions(badgecpp PRIVATE "BADGECPP_VERSION_STRING=\"${BADGECPP_VERSION_STRING}\"")

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_definitions(badgecpp PRIVATE "BADGECPP_BUILD_TYPE=\"DEBUG\"")
else()
    target_compile_definitions(badgecpp PRIVATE "BADGECPP_BUILD_TYPE=\"RELEASE\"")
endif()

if(MSVC)
    target_compile_options(badgecpp PRIVATE /W4 /WX
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Release>:/O2>)
else()
    target_compile_options(badgecpp PRIVATE -Wall -Werror
        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:Release>:-O3>)
endif()

# Execute assets build script
find_package(Python3 REQUIRED COMPONENTS Interpreter)

if(NOT Python3_FOUND)
    message(FATAL_ERROR "Python 3 not found. Please install Python 3.")
endif()

add_custom_target(
    build_assets
    COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/tools/icons_tar.py"
    COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/tools/resource-builder.py"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/icons_tar.py" "${CMAKE_CURRENT_SOURCE_DIR}/tools/resource-builder.py"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building assets using Python scripts"
)

add_dependencies(badgecpp build_assets)
execute_process(
    COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/tools/icons_tar.py"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/tools/resource-builder.py"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
)
add_subdirectory(assets/build)
target_link_libraries(badgecpp PRIVATE BadgeAssets::BadgeAssets)

install(TARGETS badgecpp
    EXPORT badgecppTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(DIRECTORY inc/ DESTINATION include)

option(BADGECPP_TESTS "Build the badgecpp test executable" ON)

if(BADGECPP_TESTS)
    add_executable(badgecpp_test "main.cpp")
    target_include_directories(badgecpp_test PRIVATE "${PROJECT_SOURCE_DIR}/inc")
    target_link_libraries(badgecpp_test badgecpp)

    target_compile_options(badgecpp_test PRIVATE
        $<$<CONFIG:Debug>:-O0 -g>
    )

    enable_testing()
    add_test(NAME BadgecppTest COMMAND badgecpp_test)
    message(STATUS "Enabled badgecpp tests")
endif()
