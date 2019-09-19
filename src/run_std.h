#ifndef RUN_INCLUDED_run_std
#define RUN_INCLUDED_run_std

#include <stdlib.h>

#include "run.h"

int run_std(RunOptions* runOptions)
{
    if(runOptions->showDebugInfos)
        puts("[debug] using implementation: std");

    puts("[warning] the std impl blocks until end of started process");

    if(!runOptions->keepIO)
    {
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);
    }

    int ret = system(runOptions->command);

    if(runOptions->showDebugInfos)
        printf("[debug] using system() call, ret: %i\n", ret);

    return 0;
}

#endif