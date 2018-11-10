add_executable(psp_test psp.cpp main.cpp)
target_link_libraries(psp_test gtest tbb tbbmalloc psp)
add_test(NAME psptest COMMAND psp_test)