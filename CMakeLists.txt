cmake_minimum_required(VERSION 3.8)
project(XO)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES pieceset.h piece.h pieceset.cpp field.h threatinfo.h fieldmetrics.h fieldmetrics.cpp threatcache.h threatcache.cpp types.h core.h core.cpp squarescore.h squarescore.cpp engineinfo.h point.h gameobserver.h gameobserver.cpp movelist.h piecesetcache.h piecesetcache.cpp squarescorecache.h squarescorecache.cpp directioniterator.h gamestate.h gamestate.cpp oldthreatcache.h oldthreatcache.cpp positionscorecache.h positionscorecache.cpp positionscore.h positionscore.cpp icoreimpl.cpp icoreimpl.h Include/icore.h)
include_directories(Include)

add_library(XO_CORE STATIC ${SOURCE_FILES})
