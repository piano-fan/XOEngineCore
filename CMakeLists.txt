cmake_minimum_required(VERSION 3.8)
project(XO)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES XO.h XO.cpp piecemask.h piece.h piecemask.cpp piece.cpp field.h field.cpp threatinfo.h threatinfo.cpp fielditerator.h fielditerator.cpp direction.h direction.cpp threatcache.h threatcache.cpp types.h square.h square.cpp core.h engineinfo.h icoreimpl.cpp icoreimpl.h Include/icore.h)
include_directories(Include)

add_library(XO_CORE STATIC ${SOURCE_FILES})
