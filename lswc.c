#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  char *path = argv[1];

  if (argc > 2)
  {
    printf("Usage: ./lswc <path>");
    return 1;
  }

  int p[2];
  pipe(p);
  int f = fork();
  if (f == 0)
  {
    dup2(p[0], 0);
    close(p[1]);
    char *args[] = {"wc", 0, NULL};
    execvp("wc", args);
    perror("Error");
    exit(1);
  }
  if (f != 0)
  {
    dup2(p[1], 1);
    close(p[0]);
    char *args[] = {"ls", "-1a", path, NULL};
    execvp("ls", args);
  }
  wait(NULL);

  return 0;
}
