cmake_minimum_required (VERSION 3.0)
project (psp)

if (NOT DEFINED PSP_WASM_BUILD)
    set(PSP_WASM_BUILD OFF)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories("${CMAKE_SOURCE_DIR}/src/include")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/install")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/install)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/install)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/install)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


if (PSP_WASM_BUILD)
	add_definitions(-DPSP_ENABLE_WASM)
	set(CMAKE_C_COMPILER emcc)
	set(CMAKE_CXX_COMPILER em++)
	set(CMAKE_TOOLCHAIN_FILE "$ENV{EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake")
	set(CMAKE_AR emar)
	set(CMAKE_RANLIB emranlib)
	set(CMAKE_EXECUTABLE_SUFFIX ".js")

	set(CMAKE_CXX_FLAGS " \
		--bind \
		--memory-init-file 0 \
		-s NO_EXIT_RUNTIME=1 \
		-s NO_FILESYSTEM=1 \
		-s ALLOW_MEMORY_GROWTH=1 \
		-s DYNAMIC_EXECUTION=2 \
		-s ASSERTIONS=2 \
		-s EXPORTED_FUNCTIONS=\"['_main']\" \
	")

    set(CMAKE_CXX_FLAGS_DEBUG " \
        -O0 \
        -g4 \
        -s SAFE_HEAP=1 \
        -s DISABLE_EXCEPTION_CATCHING=0 \
        -s ASSERTIONS=2 \
        -s DEMANGLE_SUPPORT=1 \
    ")

	set(SYNC_MODE_FLAGS "-s ENVIRONMENT=node -s BINARYEN_ASYNC_COMPILATION=0 -s BINARYEN_METHOD='\"native-wasm\"' -s WASM=1")
	set(ASYNC_MODE_FLAGS "-s ENVIRONMENT=worker -s BINARYEN_ASYNC_COMPILATION=1 -s BINARYEN_METHOD='\"native-wasm\"' -s WASM=1")
endif()

set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

set (SOURCE_FILES
src/cpp/aggregate.cpp
src/cpp/aggspec.cpp
src/cpp/arg_sort.cpp
src/cpp/base.cpp
src/cpp/base_impl_linux.cpp
src/cpp/base_impl_osx.cpp
src/cpp/base_impl_win.cpp
src/cpp/build_filter.cpp
#src/cpp/calc_agg_dtype.cpp
src/cpp/column.cpp
src/cpp/comparators.cpp
src/cpp/compat.cpp
src/cpp/compat_impl_linux.cpp
src/cpp/compat_impl_win.cpp
src/cpp/config.cpp
src/cpp/context_base.cpp
src/cpp/context_grouped_pkey.cpp
src/cpp/context_handle.cpp
src/cpp/context_one.cpp
src/cpp/context_two.cpp
src/cpp/context_zero.cpp
src/cpp/custom_column.cpp
src/cpp/data.cpp
src/cpp/date.cpp
src/cpp/dense_nodes.cpp
src/cpp/dense_tree_context.cpp
src/cpp/dense_tree.cpp
src/cpp/dependency.cpp
src/cpp/extract_aggregate.cpp
src/cpp/filter.cpp
src/cpp/flat_traversal.cpp
src/cpp/gnode.cpp
src/cpp/gnode_state.cpp
src/cpp/histogram.cpp
src/cpp/kernel_engine.cpp
src/cpp/logtime.cpp
src/cpp/mask.cpp
src/cpp/min_max.cpp
src/cpp/multi_sort.cpp
src/cpp/none.cpp
src/cpp/path.cpp
src/cpp/pivot.cpp
src/cpp/pool.cpp
src/cpp/port.cpp
src/cpp/raii.cpp
src/cpp/raii_impl_linux.cpp
src/cpp/raii_impl_win.cpp
src/cpp/range.cpp
src/cpp/rlookup.cpp
src/cpp/scalar.cpp
src/cpp/schema_column.cpp
src/cpp/schema.cpp
src/cpp/slice.cpp
src/cpp/sort_specification.cpp
src/cpp/sparse_tree.cpp
src/cpp/sparse_tree_node.cpp
src/cpp/step_delta.cpp
src/cpp/storage.cpp
src/cpp/storage_impl_linux.cpp
src/cpp/storage_impl_win.cpp
src/cpp/sym_table.cpp
src/cpp/table.cpp
src/cpp/time.cpp
src/cpp/traversal.cpp
src/cpp/traversal_nodes.cpp
src/cpp/tree_context_common.cpp
src/cpp/utils.cpp
src/cpp/update_task.cpp
src/cpp/vocab.cpp
)

file(GLOB HEADER_FILES src/include/perspective/*.h)
if (PSP_WASM_BUILD)
	add_library(psp STATIC ${SOURCE_FILES} ${HEADER_FILES})
else()
	add_library(psp SHARED ${SOURCE_FILES} ${HEADER_FILES})
endif()

if (NOT PSP_WASM_BUILD)
    if (UNIX)
        target_compile_options(psp PRIVATE -Wall -pedantic -Werror)
        target_compile_options(psp PRIVATE $<$<CONFIG:DEBUG>:-fPIC -O0>)
    endif()
    target_compile_definitions(psp PRIVATE $<$<CONFIG:DEBUG>:D_GLIBCXX_DEBUG PSP_DEBUG>)
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		target_compile_options(psp PRIVATE $<$<CONFIG:DEBUG>:-fprofile-instr-generate -fcoverage-mapping>)
        target_link_libraries(psp PRIVATE $<$<CONFIG:DEBUG>:--coverage>)
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		target_compile_options(psp PRIVATE -time=build.time -Wno-deprecated-declarations -Wno-deprecated)
		target_compile_options(psp PRIVATE $<$<CONFIG:DEBUG>:-fprofile-arcs -ftest-coverage>)
		target_link_libraries(psp PRIVATE $<$<CONFIG:DEBUG>:--coverage>)
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	endif()

	add_custom_target(gcov
           COMMAND mkdir -p ${CMAKE_BINARY_DIR}/coverage
           WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
           )
   	add_custom_command(TARGET gcov
           COMMAND echo "=================== GCOV ===================="
		   COMMAND gcovr -r ../ --html --html-details -o coverage/index.html
           COMMAND xdg-open coverage/index.html
           WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
           )
endif()


if (WIN32)
	target_compile_definitions(psp PUBLIC -DPERSPECTIVE_EXPORTS)
	foreach(warning 4244 4251 4267 4275 4290 4786 4305 4996)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd${warning}")
	endforeach(warning)
endif()

set_target_properties(psp PROPERTIES COMPILE_FLAGS "${ASYNC_MODE_FLAGS}")

set(PSP_DIST_DIR ${CMAKE_SOURCE_DIR}/dist)

if (PSP_WASM_BUILD)
	add_executable(perspective.async src/cpp/main.cpp)
	target_link_libraries(perspective.async psp "${ASYNC_MODE_FLAGS}")
	set_target_properties(perspective.async PROPERTIES COMPILE_FLAGS "${ASYNC_MODE_FLAGS}")
	set_target_properties(perspective.async PROPERTIES OUTPUT_NAME "psp.async")

	add_executable(perspective.sync src/cpp/main.cpp)
	target_link_libraries(perspective.sync psp "${SYNC_MODE_FLAGS}")
	set_target_properties(perspective.sync PROPERTIES COMPILE_FLAGS "${SYNC_MODE_FLAGS}")
	set_target_properties(perspective.sync PROPERTIES OUTPUT_NAME "psp.sync")

	add_custom_target(jsbuild
           COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/dist/js/lib
		   COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/dist/js/staging
           WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

	add_custom_command(TARGET jsbuild
           COMMAND echo "=================== JSBUILD ===================="
		   COMMAND cp build/install/psp.sync.wasm dist/js/
		   COMMAND cp build/install/psp.async.wasm dist/js/
		   COMMAND cp build/install/psp.async.js dist/js/staging
		   COMMAND cp build/install/psp.sync.js dist/js/staging
		   COMMAND cat dist/js/staging/pre dist/js/staging/psp.async.js dist/js/staging/post > bindings/typescript/psp.async.js
		   COMMAND cat dist/js/staging/pre dist/js/staging/psp.sync.js dist/js/staging/post > bindings/typescript/psp.sync.js
		   COMMAND rm dist/js/staging/psp.async.js
		   COMMAND rm dist/js/staging/psp.sync.js
		   COMMAND yarn build
		   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

	add_dependencies(jsbuild perspective.sync perspective.async)
endif()


function (psp_build_dep name cmake_file)
	configure_file(${cmake_file} ${name}-download/CMakeLists.txt)
	execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
			RESULT_VARIABLE result
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${name}-download )
	if(result)
		message(FATAL_ERROR "CMake step for ${name} failed: ${result}")
	endif()
	execute_process(COMMAND ${CMAKE_COMMAND} --build .
			RESULT_VARIABLE result
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${name}-download )
	if(result)
		message(FATAL_ERROR "Build step for ${name} failed: ${result}")
	endif()
	add_subdirectory(${CMAKE_BINARY_DIR}/${name}-src
			${CMAKE_BINARY_DIR}/${name}-build
			EXCLUDE_FROM_ALL)
	include_directories(${CMAKE_BINARY_DIR}/${name}-src/extras/gtest/include)
    include_directories(${CMAKE_BINARY_DIR}/${name}-src/include)
	include_directories(${CMAKE_BINARY_DIR}/${name}-src)
endfunction()

if (NOT PSP_WASM_BUILD)
   if (WIN32)
       set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
   endif()
   set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
   psp_build_dep("googletest" "cmake/GTest.txt.in")
   #psp_build_dep("tbb" "cmake/TBB.txt.in")
   #psp_build_dep("benchmark" "cmake/benchmark.txt.in")
   add_subdirectory(test)
   #add_subdirectory(bench)
endif()

