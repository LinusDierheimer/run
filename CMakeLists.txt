cmake_minimum_required(VERSION 3.0)

project(run)

if(UNIX)

	add_executable(run 
		src/main.c
		src/run_std.c
		src/run_unix.c
	)

	install(
		TARGETS run
		DESTINATION "/usr/local/bin"
	)

elseif(WIN32)

	add_executable(run 
		src/main.c
		src/run_std.c
		src/run_windows.c
	)

	set(CMAKE_C_COMPILER_WORKS 1)

else(UNIX)
	
	add_executable(run
		src/main.c
		src/run_std.c
		src_run_std_adapter.c
	)

endif(UNIX)