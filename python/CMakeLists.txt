file(GLOB_RECURSE SOURCES "src/**")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 REQUIRED)
pybind11_add_module(_pybadge ${SOURCES})

target_link_libraries(_pybadge PRIVATE badgecpp)

# set_target_properties(_pybadge PROPERTIES SUFFIX ".so")