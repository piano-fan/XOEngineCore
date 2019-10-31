cmake_minimum_required(VERSION 3.8)
project(XO)

set(CMAKE_CXX_STANDARD 17)

set(OBSERVER_FILES Observer/pieceset.h Observer/threat.h Observer/threatcache.h Observer/threatcache.cpp Observer/squarescore.h Observer/squarescore.cpp Observer/squareobserver.h Observer/squareobserver.cpp Observer/oldthreatcache.h Observer/sortedthreatset.h Observer/squareinfluence.h Observer/squareinfluence.cpp Observer/tacticset.h Observer/tacticalcache.h Observer/tacticalcache.cpp  Observer/squaretracker.h Observer/positioncache.h)
set(CORE_FILES engineinfo.h icoreimpl.h icoreimpl.cpp staticevaluation.h icoreimpl.cpp icoreimpl.h Include/icore.h Include/builtintypes.h Include/fieldmetrics.h Include/move.h Include/offset_1d.h Include/piece.h Include/point.h Include/staroffset.h Include/math.h baseevaluator.h evaluationmanager.h statictactics.h tactics_deprecated.h)
include_directories(Include, Observer)

add_library(XO_OBSERVER STATIC ${OBSERVER_FILES})
add_library(XO_CORE STATIC ${CORE_FILES})
target_link_libraries(XO_CORE XO_OBSERVER)


