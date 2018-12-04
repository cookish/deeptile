cmake_minimum_required(VERSION 3.12)
project(mltwo)

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
        )

add_library(mltwo_lib ${SOURCES})
add_executable(mltwo main.cpp)
target_link_libraries(mltwo mltwo_lib)

add_subdirectory(tests)
