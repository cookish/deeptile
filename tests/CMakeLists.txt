set (TESTS testMain.cc testBoard.cc testRow.cc)

add_executable(testMain ${TESTS})
target_link_libraries(testMain mltwo_lib)