#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int     main(int argc, char **argv)
{
    pid_t   pid;
    int     status;
    int     out;
    char    *cmd[3];

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (1);
    }
    if (pid == 0)
    {
        out = open("out.log", O_WRONLY | O_CREAT, 0666);
        if (out == -1)
        {
            perror("open");
            return (1);
        }
        if (dup2(out, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            return (1);
        }
        cmd[0] = "ps";
        cmd[1] = "aux";
        cmd[2] = NULL;
        execvp(cmd[0], cmd);
        perror("execvp");
    }
    else 
    {
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid");
            return (1);
        }
        if (WIFEXITED(status)) 
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        else 
            printf("Child did not exit cleanly\n");
    }
    return (0);
}