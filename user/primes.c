#include "kernel/types.h"
#include "user/user.h"

void pipeline(int fd);

void main()
{
  int p[2];
  pipe(p);
  for (int i = 2; i < 35; i++)
  {
    write(p[1], &i, sizeof(i));
  }
  close(p[1]);
  pipeline(p[0]);
}

void pipeline(int rd)
{
  int a = 0, b;
  int p[2];
  pipe(p);
  if (fork() == 0)
  {
    close(p[1]);
    pipeline(p[0]);
  }
  else
  {
    close(p[0]);
    while (read(rd, &b, sizeof(b)) == sizeof(b))
    {
      if (a == 0)
      {
        printf("prime %d\n", a = b);
      }
      else if (b % a)
      {
        write(p[1], &b, sizeof(b));
      }
    }
    close(p[1]);
    close(rd);
    wait(0);
    exit(0);
  }
}
