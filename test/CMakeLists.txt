set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -fPIC -O0")
add_executable(psp_test psp.cpp)
target_link_libraries(psp_test gtest_main tbb tbbmalloc psp)
add_test(NAME psptest COMMAND psp_test)