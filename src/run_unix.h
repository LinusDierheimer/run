#ifndef RUN_INCLUDED_run_unix
#define RUN_INCLUDED_run_unix

#include <stdbool.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int run(char* command, bool debugInfos, bool keepIO)
{

    if(debugInfos)
        puts("[debug] using implementation: unix");

    int ret = fork();

    if(debugInfos)
        if(ret > 0)
            printf("[debug] using new pid: %i\n", ret);

    if(ret == -1)
    {
        fprintf(stderr, "[error] fork returned -1\n");
        return 1;
    }
    else if(ret == 0)
    {
        if(keepIO != true)
        {
            int fd = open("/dev/null", O_RDWR, 0);
            if(debugInfos)
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

            umask(027);
        }
        
        execlp("sh", "sh", "-c", command, NULL);
    }
    
    return 0;

}

#endif