#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
  char b[1];
  int p[2];
  pipe(p);
  if (fork() == 0)
  {
    read(p[0], b, sizeof(b));
    printf("%d: received ping\n", getpid());
    write(p[1], b, sizeof(b));
    exit(0);
  }
  else
  {
    write(p[1], b, sizeof(b));
    read(p[0], b, sizeof(b));
    printf("%d: received pong\n", getpid());
    exit(0);
  }
}