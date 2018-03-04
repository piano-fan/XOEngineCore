cmake_minimum_required(VERSION 3.8)
project(XO)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp XO.h XO.cpp piecemask.h piece.h piecemask.cpp piece.cpp field.h field.cpp threatinfo.h threatinfo.cpp fielditerator.h fielditerator.cpp direction.h direction.cpp core.h core.cpp engineinfo.h)

add_executable(XO ${SOURCE_FILES})
