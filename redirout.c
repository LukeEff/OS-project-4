#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    char *command = argv[2];
    int output = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (argc < 4)
    {
        printf("Usage: ./redirout <out-file> <command> <options> <argument>");
        return 1;
    }

    int p[2];
    pipe(p);
    int f = fork();
    if (f == 0)
    {
        dup2(p[1], 1);
        close(p[0]);
        execvp(command, argv + 2);
        perror("Error");
        exit(1);
    }
    if (f != 0)
    {
        close(p[1]);

        char *buf[2048];
        int count = read(p[0], buf, sizeof buf);

        while (count > 0)
        {
            write(output, buf, sizeof buf);
            count = read(p[0], buf, sizeof buf);
        }

        close(p[0]);
        close(output);
    }
    wait(NULL);

    return 0;
}
