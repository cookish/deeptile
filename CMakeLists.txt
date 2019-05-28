cmake_minimum_required(VERSION 3.0)
project(deeptile)

include_directories(include)
include_directories(externals/inih)
include_directories(externals/inih/cpp)
add_subdirectory(externals)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads)

set(SOURCES
        src/BoardHandler.cc
        src/RowHandler.cc
        src/HeuristicScorer.cc
        src/ExpectiMax.cc
        src/Utility.cc
        src/GameStats.cc
        src/RunStats.cc
        src/JsonStats.cc
        src/Output.cc
        src/Settings.cc
        src/DataLogger.cc
        src/MLScorer.cc
        )

if(NOT Boost_FOUND)
    FIND_PACKAGE( Boost COMPONENTS filesystem REQUIRED )
endif()
include_directories(${Boost_INCLUDE_DIRS})

find_package(pybind11 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(CURL REQUIRED)

add_library(deeptile_lib ${SOURCES})
add_executable(deeptile main.cpp)
target_link_libraries(deeptile PRIVATE pybind11::embed)
target_link_libraries(deeptile PRIVATE deeptile_lib ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(deeptile PRIVATE inih_lib ${Boost_LIBRARIES})
target_link_libraries(deeptile PRIVATE ${CURL_LIBRARIES})

add_executable(testDocker testDocker.cc)
target_link_libraries(testDocker ${CURL_LIBRARIES})

pybind11_add_module(heuristicMod  heuristicMod.cc)
target_link_libraries(heuristicMod PRIVATE pybind11::module deeptile_lib inih_lib)
#install(TARGETS heuristicMod DESTINATION ...?)

add_subdirectory(tests)
