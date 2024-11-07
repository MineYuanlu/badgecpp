file(GLOB_RECURSE SOURCES "src/**")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 REQUIRED)
pybind11_add_module(pybadge ${SOURCES})

target_link_libraries(pybadge PRIVATE badgecpp)