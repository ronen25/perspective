add_executable(psp_test psp.cpp main.cpp)
target_link_libraries(psp_test gtest tbb tbbmalloc psp)

add_executable(scratch scratch.cpp)
target_link_libraries(scratch gtest tbb tbbmalloc psp)