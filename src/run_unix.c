#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int run(int argc, char** command)
{
    int ret = fork();

    if(ret == -1)
    {
        fprintf(stderr, "fork returned -1");
        return 1;
    }
    else if(ret == 0)
    {
        int fd = open("/dev/null", O_RDWR, 0);
        if (fd >= 0)
        {
            dup2 (fd, STDIN_FILENO);
            dup2 (fd, STDOUT_FILENO);
            dup2 (fd, STDERR_FILENO);
            if (fd > 2)
            {
                close (fd);
            }
        }

        umask(027);

        char buf[4096];

        for(int i = 0; i < argc; i++)
        {
            strcat(buf, command[i]);
            strcat(buf, " ");
        }

        execlp("sh", "sh", "-c", buf, NULL);
    }
    
    return 0;

}