cmake_minimum_required(VERSION 3.8)
project(XO)

set(CMAKE_CXX_STANDARD 20)

set(OBSERVER_FILES squarescore.h squarescore.cpp squareobserver.h squareobserver.cpp sortedthreatset.h squareinfluence.h squareinfluence.cpp)

add_library(XO_OBSERVER STATIC ${OBSERVER_FILES})
add_subdirectory(Threat)

target_include_directories(XO_OBSERVER PUBLIC Threat)
target_link_libraries(XO_OBSERVER XO_THREAT)
