cmake_minimum_required(VERSION 3.12)
project(deeptile)

include_directories(include)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        src/BoardHandler.cc
        src/RowHandler.cc
        src/HeuristicScorer.cc
        src/ExpectiMax.cc
        src/Utility.cc
        src/ScoreCache.cc
        src/GameStats.cc
        src/RunStats.cc
        src/JsonStats.cc
        )

add_library(deeptile_lib ${SOURCES})
add_executable(deeptile main.cpp)
target_link_libraries(deeptile deeptile_lib)

add_subdirectory(tests)
