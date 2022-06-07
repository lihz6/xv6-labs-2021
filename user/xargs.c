#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  char buf[1024];
  char *args[64];
  char *p = buf, **a = args;
  for (int i = 1; i < argc; i++)
  {
    *a++ = argv[i];
  }
  *a = 0;
  while (read(0, p, 1) > 0)
  {
    if (p >= buf + sizeof(buf) - 1)
    {
      fprintf(2, "xargs: buf too small(%d)", sizeof(buf));
      exit(1);
    }
    if (a >= args + sizeof(args) - 1)
    {
      fprintf(2, "xargs: args too small(%d)", sizeof(args));
      exit(1);
    }
    if (*p == ' ' || *p == '\n' || *p == '\r')
    {
      if (*a != 0)
      {
        *++a = 0;
        *p++ = 0;
      }
    }
    else
    {
      if (*a == 0)
      {
        *a = p;
      }
      p++;
    }
  }
  if (*p != 0)
  {
    *++a = 0;
    *++p = 0;
  }
  if (fork() == 0)
  {
    exec(args[0], args);
  }
  wait(0);
  exit(0);
}