#!/usr/bin/env python3

import sys
import os
import subprocess

def get_status_output(*args, **kwargs):
    p = subprocess.Popen(*args, **kwargs, shell=True)
    stdout, stderr = p.communicate()
    return p.returncode, stdout, stderr

def exec(*args, **kwargs):
	rc, stdout, stderr = get_status_output(*args, **kwargs)
	if rc:
		print('== STDOUT ==')
		print(stdout)

		print('== STDERR ==')
		print(stderr)

		sys.exit(rc)

# Following environment variables will be passed in 
# compiler in (clang, gcc, emscripten)
# build_type (release, debug, asan, msan, tsan)

build_type_map = {
	'Release': 'Release',
	'Debug': 'Debug',
	'Asan': 'Release',
	'Msan': 'Release',
	'Tsan': 'Release',
	'Valgrind': 'Release',
	'ValgrindDbg': 'Debug'
}

release_flag_map = {
	'Release': '-O3',
	'Debug': '-ggdb3',
	'Asan': '-O1 -g -fsanitize=address -fno-omit-frame-pointer',
	'Msan': '-fsanitize=memory -fno-omit-frame-pointer -g -O2',
	'Tsan': '-fsanitize=thread -g -O1',
	'Valgrind': '',
	'ValgrindDbg': ''
}

compiler_definitions_map = {
	'clang': '',
	'gcc': '',
	'emscripten': '-DPSP_WASM_BUILD=ON'
}

compiler_c_map = {
	'clang': '/usr/bin/clang',
	'gcc': 'gcc',
	'emscripten': '/usr/bin/clang'
}

compiler_cpp_map = {
	'clang': '/usr/bin/clang++',
	'gcc': 'g++',
	'emscripten': '/usr/bin/clang++'
}


def main():
	build_dir = 'build_' + os.environ['Compiler'] + '_' + os.environ['BuildType']
	exec(['mkdir -p ' + build_dir])

	if os.environ['Compiler'] == 'gcc' and os.environ['BuildType'] == 'Msan':
		sys.exit(0)

	os.chdir(build_dir)

	build_type = build_type_map[os.environ['BuildType']]
	flags = release_flag_map[os.environ['BuildType']]
	definitions = compiler_definitions_map[os.environ['Compiler']]
	cc = compiler_c_map[os.environ['Compiler']]
	cxx = compiler_cpp_map[os.environ['Compiler']]

	cmd1 = 'cmake -G Ninja -DPSP_BOOST_INCLUDE="/usr/include" -DCMAKE_BUILD_TYPE=%s -DCMAKE_CXX_FLAGS="%s" ' % (build_type, flags)
	cmd2 = '-DCMAKE_C_COMPILER=%s -DCMAKE_CXX_COMPILER=%s %s ..' % (cc, cxx, definitions)
	print('compile_command: ' + cmd1 + cmd2)
	exec([cmd1 + cmd2])
	exec(['ninja'])

	if os.environ['Compiler'] == 'emscripten':
		sys.exit(0)

	if os.environ['BuildType'] in ('Valgrind', 'ValgrindDbg'):
		test_cmd = 'valgrind --leak-check=full --verbose --track-origins=yes ./install/psp_test '
	else:
		test_cmd = './install/psp_test'
	exec([test_cmd,])
	sys.exit(0)


if __name__ == '__main__':
	main()
