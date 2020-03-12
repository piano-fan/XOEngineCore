cmake_minimum_required(VERSION 3.8)
project(XO)

set(CMAKE_CXX_STANDARD 20)

set(CORE_FILES engineinfo.h icoreimpl.h icoreimpl.cpp staticevaluation.h Include/icore.h Include/builtintypes.h Include/types.h Include/fieldmetrics.h Include/move.h Include/offset_1d.h Include/piece.h Include/point.h Include/staroffset.h Include/math.h evaluationmanager.h report.h statictactics.h tactics_deprecated.h movegenerator.h variationmanager.h positioncache.h)

add_library(XO_CORE STATIC ${CORE_FILES})
add_subdirectory(Observer)

target_include_directories(XO_CORE PUBLIC Observer)
target_link_libraries(XO_CORE XO_OBSERVER)
