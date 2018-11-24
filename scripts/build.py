#!/usr/bin/env python3

import sys
import os
import commands

# Following environment variables will be passed in 
# compiler in (clang, gcc, emscripten)
# build_type (release, debug, asan, msan, tsan)

build_type_map = {
	'release': 'Release',
	'debug': 'Debug',
	'asan': 'Release',
	'msan': 'Release',
	'tsan': 'Release',
	'valgrind': 'Release',
	'valgrind_dbg': 'Debug'
}

release_flag_map = {
	'release': '-O3',
	'debug': '-ggdb3',
	'asan': '-O1 -g -fsanitize=address -fno-omit-frame-pointer',
	'msan': '-fsanitize=memory -fno-omit-frame-pointer -g -O2',
	'tsan': '-fsanitize=thread -g -O1',
	'valgrind': '',
	'valgrind_dbg': ''
}

compiler_definitions_map = {
	'clang': '',
	'gcc': '',
	'emscripten': '-DPSP_WASM_BUILD'
}

compiler_c_map = {
	'clang': 'clang',
	'gcc': 'gcc',
	'emscripten': 'emcc'
}

compiler_cpp_map = {
	'clang': 'clang++',
	'gcc': 'g++',
	'emscripten': 'em++'
}



def main():

	def handle_exit(rc, op):
		if rc:
			print(op)
			sys.exit(rc)

	rc, op = commands.getstatusoutput('mkdir build')
	handle_exit(rc, op)

	os.chdir('build')

	release = build_type_map[os.environ['BuildType']]
	flags = release_flag_map[os.environ['BuildType']]
	definitions = compiler_definitions_map[os.environ['Compiler']]
	cc = compiler_c_map[os.environ['Compiler']]
	cxx = compiler_cpp_map[os.environ['Compiler']]

	rc, op = commands.getstatusoutput('cmake -G Ninja -DCMAKE_BUILD_TYPE=%s -DCMAKE_CXX_FLAGS="%s" %s -DCMAKE_C_COMPILER=%s -DCMAKE_CXX_COMPILER=%s ..' % (release, flags, definitions, cc, cxx))
	handle_exit(rc, op)

	rc, op = commands.getstatusoutput('ninja')
	handle_exit(rc, op)

	if os.environ['Compiler'] == 'emscripten':
		sys.exit(0)

	rc, op = commands.getstatusoutput('./install/psp_test')
	sys.exit(rc)


if __name__ == '__main__':
	main()
