cmake_minimum_required(VERSION 3.0)
project(deeptile)

include_directories(include)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads)

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
        src/Output.cc
        )

add_library(deeptile_lib ${SOURCES})
add_executable(deeptile main.cpp)
target_link_libraries(deeptile deeptile_lib ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(tests)
