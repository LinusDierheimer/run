#ifndef RUN_INCLUDED_run_unix
#define RUN_INCLUDED_run_unix

#include <sys/types.h>  
#include <sys/stat.h>
#include <sys/wait.h>  
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "run.h"

static const int ERROR_FORK_FAILED = 31;
static const int ERROR_EXEC_RETURNED = 32;

static void dummy_signal_handler(int signo) { return; }

static void redirect_io(RunOptions* runOptions)
{
    if(runOptions->keepIO)
        return;

    int fd = open("/dev/null", O_RDWR, 0);

    if(runOptions->showDebugInfos)
        printf("[debug] using file descriptor /dev/null: %i\n", fd);

    if (fd >= 0)
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > 2)
        {
            close (fd);
        }
    }
    else
    {
        printf("[warning] open(\"/dev/null\", O_RDWR, 0) returned %i, closing streams\n", fd);
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);
    }
    
}

static int handle_parent(RunOptions* runOptions, int childpid)
{
    if(runOptions->printChildPID)
    {
        printf("[run] child-pid: %i\n", childpid);
    }

    if(runOptions->waitForFinish || runOptions->printExitCode)
    {
        int stat = 0;
        wait(&stat);

        if(runOptions->printExitCode)
            printf("[run] child-exit-code: %i\n", WEXITSTATUS(stat));
    }
    else
    {
        pause();
    }
    
    return 0;
}

static int handle_child(RunOptions* runOptions)
{

    signal(SIGTERM, SIG_DFL);

    kill(getppid(), SIGTERM);

    if(runOptions->terminal)
    {
        char* desktop = getenv("DESKTOP_SESSION");

        if(runOptions->showDebugInfos)
            printf("[debug] using desktop environment: %s\n", desktop == NULL ? "NULL" : desktop);

        if(desktop != NULL)
        {
            char* console;

            if(strcmp(desktop, "plasma") == 0)
                console = "konsole";

            if(runOptions->showDebugInfos)
                printf("[debug] using console command: %s\n", console == NULL ? "NULL" : console);

            if(console != NULL)
            {
                redirect_io(runOptions);

                if(runOptions->runAsRoot)
                    execlp(console, console, "--separate", "--hold", "-e", "sudo", "sh", "-c", runOptions->command, NULL);
                else
                    execlp(console, console, "--separate", "--hold", "-e", "sh", "-c", runOptions->command, NULL);

                return ERROR_EXEC_RETURNED;
            }
            else
            {
                puts("[warning] couldn't detect terminal. continuing without");
            }
        }
        else
        {
            puts("[waring] couldn't detect desktop environment. continuing without terminal");
        }
        
    }

    redirect_io(runOptions);

    if(runOptions->runAsRoot)
        execlp("sudo", "sudo", "sh", "-c", runOptions->command, NULL);
    else
        execlp("sh", "sh", "-c", runOptions->command, NULL);

    return ERROR_EXEC_RETURNED;
}

int run(RunOptions* runOptions)
{
    if(runOptions->showDebugInfos)
        puts("[debug] using implementation: unix");

    if(!runOptions->waitForFinish)
        signal(SIGINT, SIG_IGN);

    signal(SIGTERM, dummy_signal_handler);

    int ret = fork();

    if(runOptions->showDebugInfos)
        if(ret > 0)
            printf("[debug] using new pid: %i\n", ret);
        
    if(ret < 0)
    {
        fprintf(stderr, "[error] fork returned < 0: %i\n", ret);
        return ERROR_FORK_FAILED;
    }
    else if(ret == 0)
        return handle_child(runOptions);
    else
        return handle_parent(runOptions, ret);
}

#endif