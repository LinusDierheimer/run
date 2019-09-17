#include <stdio.h>
#include "run.h"

int main(int argc, char** argv)
{
    if(argc < 2)
        fprintf(stderr, "run must be started with a command to execute\n");
    
    return run(argc - 1, &argv[1]);
}