#include "run_std.h"

#include <stdio.h>
#include <stdlib.h>

int run_std(RunOptions* runOptions)
{
    if(runOptions->showDebugInfos)
        puts("[debug] using implementation: std");

    #if defined(__unix__)
        puts("[warning] you seem to be on a unix platform, run has a special implementation for it which should be used, remove the --std-impl / -s flag to do so.");
    #elif defined(_WIN32)
		puts("[warning] you seem to be on a windows platform, run has a special implementation for it which should be used, remove the --std-impl / -s flag to do so.");
	#endif

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