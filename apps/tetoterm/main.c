#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <pty.h>

int main(void)
{
    int master;
    pid_t pid;

    pid = forkpty(&master, NULL, NULL, NULL);

    if (pid < 0)
    {
        perror("forkpty");
        return 1;
    }

    if (pid == 0)
    {
        execl("/bin/sh", "sh", NULL);

        perror("execl");
        _exit(127);
    }

    while (1)
    {
        char buffer[4096];
        ssize_t count;

        count = read(master, buffer, sizeof(buffer));

        if (count <= 0)
            break;

        if (write(STDOUT_FILENO, buffer, count) < 0)
            break;
    }

    close(master);

    waitpid(pid, NULL, 0);

    return 0;
}