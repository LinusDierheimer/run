cmake_minimum_required(VERSION 3.0)

if(UNIX)

    add_executable(run 
        src/main.c
        src/run_unix.c
    )

    install(
        TARGETS run
        DESTINATION "/usr/bin"
    )
    
endif(UNIX)
