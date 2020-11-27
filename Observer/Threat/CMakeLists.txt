cmake_minimum_required(VERSION 3.8)
project(XO)

set(CMAKE_CXX_STANDARD 20)

set(XO_THREAT_FILES pieceset.h threat.h threat.cpp threatcache.h threatcache.cpp oldthreatcache.h)

add_library(XO_THREAT STATIC ${XO_THREAT_FILES})

target_include_directories(XO_THREAT PUBLIC ../../Include ../../../../)
