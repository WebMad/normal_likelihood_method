cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "build/.*")

add_executable(main ${SOURCES})

find_package(Boost REQUIRED)
include_directories(
    ${BOOST_INCLUDE_DIRS}
)
link_libraries(${BOOST_LIBRARIES})
target_link_libraries(main ${BOOST_LIBRARIES})


find_package(NLopt REQUIRED)
include_directories(${NLOPT_INCLUDE_DIRS})
link_libraries(${NLOPT_LIBRARIES})
target_link_libraries(main ${NLOPT_LIBRARIES})