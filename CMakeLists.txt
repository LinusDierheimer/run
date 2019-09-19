cmake_minimum_required(VERSION 3.0)

add_executable(run 
    src/main.c
)

install(
    TARGETS run
    DESTINATION "/usr/local/bin"
)