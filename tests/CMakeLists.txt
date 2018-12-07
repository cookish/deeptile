set (TESTS testMain.cc testBoard.cc testRow.cc testHeuristicScorer.cc)

add_executable(testMain ${TESTS})
target_link_libraries(testMain deeptile_lib)