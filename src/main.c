#include <stdio.h>
#include <unistd.h>

#ifdef __unix__
    #include "run_unix.h"
#else
    #include "run_std.h"
#endif

int printHelp()
{
    puts(
        "run help\n"
        "\n"
        "--help, -h:      prints this message \n"
        "--keep-io, -k:   don't redirect io streams \n"
        "\n"
        "     this run command is inspired by running turtles"
    );
}

int main(int argc, char** argv)
{
    int argvIndex = 1;

    bool keepIO = false;
    bool debugInfos = false;

    for(; argvIndex < argc; argvIndex++)
    {
        if(strcmp(argv[argvIndex], "--help") == 0 || strcmp(argv[argvIndex], "-h") == 0)
        {
            return printHelp(); //ends process
        }
        else if(strcmp(argv[argvIndex], "--keep-io") == 0 || strcmp(argv[argvIndex], "-k") == 0)
        {
            keepIO = true;
            continue;
        }
        else if(strcmp(argv[argvIndex], "--debug-info") == 0 || strcmp(argv[argvIndex], "-d") == 0)
        {
            debugInfos = true;
            continue;
        }
        else
        {
            break;
        }
    }

    if(debugInfos)
    {
        printf("[debug] using debugInfos: true\n");
        printf("[debug] using keepIO: %s\n", keepIO ? "true" : "false");
    }

    if(argvIndex >= argc - 1)
    {
        fprintf(stderr, "[error] run must be started with a command to execute\n");
        return 0;
    }

    char buf[4096];
    buf[0] = '\0';

    for(; argvIndex < argc; argvIndex++)
    {
        strcat(buf, argv[argvIndex]);
        strcat(buf, " ");
    }

    if(debugInfos)
        printf("[debug] using command: %s\n", buf);

    return run(buf, debugInfos, keepIO);

}