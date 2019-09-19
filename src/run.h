#ifndef RUN_INCLUDED_run
#define RUN_INCLUDED_run

#include <stdbool.h>

typedef struct RunOptions
{
    char* command;
    bool showDebugInfos;
    bool keepIO;
    bool waitForFinish;
    bool printExitCode;
    bool printChildPID;
    bool runAsRoot;
} RunOptions;

#endif