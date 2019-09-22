#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "run.h"
#include "run_std.h"

#ifdef __unix__
    #include "run_unix.h"
#else
    int(*run)(RunOptions*) = run_std;
#endif

static const int ERROR_NO_COMMAND = 20;

int printHelp()
{
    puts(
        "run help\n"
        "\n"
        "This command can be used to run a shell command with settable circumstances.\n"
        "The main motivation behind it is to run apps from terminal without having the blocking output.\n"
        "It also includes optional features, listed below.\n"
        "\n"
        "commands:\n"
        "   --help,              -h: prints this message \n"
        "   --keep-io,           -k: don't close / redirect to null io streams \n"
        "   --std-impl,          -s: uses the impl based on the standard library\n"
        "   --wait,              -w: waits until the called process finished\n"
        "   --exit-code,         -e: prints the exit code, automatically sets --wait\n"
        "   --pid,               -p: prints the pid of the child process\n"
        "   --root-user, --sudo, -r: executes the command as root user \n"
        "   --terminal,          -t starts a new terminal for the process\n"
        "   --command            -c everything after this flag is treated as command, so it should be last"
        "   --debug,             -d: show debug informations\n"
        "\n"
        "source:\n"
        "   https://github.com/LinusDierheimer/run\n"
        "\n"
        "       this run command is inspired by running turtles"
    );

    return 0;
}

int main(int argc, char** argv)
{
    int argvIndex = 1;

    bool stdImpl = false;

    RunOptions runOptions = {
        .command = "",
        .showDebugInfos = false,
        .keepIO = false,
        .waitForFinish = false,
        .printExitCode = false,
        .printChildPID = false,
        .runAsRoot = false,
        .terminal = false
    };

    for(; argvIndex < argc; argvIndex++)
    {
        if(strcmp(argv[argvIndex], "--command") == 0 || strcmp(argv[argvIndex], "-c") == 0)
            break;
        else if(strcmp(argv[argvIndex], "--help") == 0 || strcmp(argv[argvIndex], "-h") == 0)
            return printHelp(); //ends process
        else if(strcmp(argv[argvIndex], "--keep-io") == 0 || strcmp(argv[argvIndex], "-k") == 0)
            runOptions.keepIO = true;
        else if(strcmp(argv[argvIndex], "--std-impl") == 0 || strcmp(argv[argvIndex], "-s") == 0)
            stdImpl = true;
        else if(strcmp(argv[argvIndex], "--debug-info") == 0 || strcmp(argv[argvIndex], "-d") == 0)
            runOptions.showDebugInfos = true;
        else if(strcmp(argv[argvIndex], "--wait") == 0 || strcmp(argv[argvIndex], "-w") == 0)
            runOptions.waitForFinish = true;
        else if(strcmp(argv[argvIndex], "--exit-code") == 0 || strcmp(argv[argvIndex], "-e") == 0)
            runOptions.printExitCode = true;
        else if(strcmp(argv[argvIndex], "--pid") == 0 || strcmp(argv[argvIndex], "-p") == 0)
            runOptions.printChildPID = true;
        else if(strcmp(argv[argvIndex], "--root-user") == 0 || strcmp(argv[argvIndex], "--sudo") == 0 || strcmp(argv[argvIndex], "-r") == 0)
            runOptions.runAsRoot = true;
        else if(strcmp(argv[argvIndex], "--terminal") == 0 || strcmp(argv[argvIndex], "-t") == 0)
            runOptions.terminal = true;
        else
            break;
    }

    if(runOptions.printExitCode)
        runOptions.waitForFinish = true;

    if(runOptions.showDebugInfos)
    {
        printf("[debug] using showDebugInfos: true\n");
        printf("[debug] using keepIO: %s\n", runOptions.keepIO ? "true" : "false");
        printf("[debug] using stdImpl: %s\n", stdImpl ? "true" : "false");
        printf("[debug] using waitForFinish: %s\n", wait ? "true" : "false");
        printf("[debug] using printExitCode: %s\n", runOptions.printExitCode ? "true" : "false");
        printf("[debug] using printChildPID: %s\n", runOptions.printChildPID ? "true" : "false");
        printf("[debug] using runAsRoot: %s\n", runOptions.runAsRoot ? "true" : "false");
        printf("[debug] using terminal: %s\n", runOptions.terminal ? "true" : "false");
    }

    if(argvIndex >= argc)
    {
        fprintf(stderr, "[error] run must be started with a command to execute\n");
        return ERROR_NO_COMMAND;
    }

    char buf[4096];
    buf[0] = '\0';

    for(; argvIndex < argc; argvIndex++)
    {
        strcat(buf, argv[argvIndex]);
        strcat(buf, " ");
    }

    runOptions.command = buf;

    if(runOptions.showDebugInfos)
        printf("[debug] using command: %s\n", runOptions.command);

    if(stdImpl)
        return run_std(&runOptions);
    else
        return run(&runOptions);

}