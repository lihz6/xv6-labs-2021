#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char buf[512];
int includes(char *path, char *name);
void find(char *path, char *name);

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(2, "find: require two arguments\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}

int includes(char *path, char *name)
{
  int n1 = (int)strlen(path);
  int n2 = (int)strlen(name);
  for (int i = 0; i <= n1 - n2; i++)
  {
    if (!memcmp(path + i, name, n2))
    {
      return 1;
    }
  }
  return 0;
}

void find(char *path, char *name)
{
  struct dirent de;
  struct stat st;
  char *p;
  int fd;

  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type)
  {
  case T_FILE:
    if (includes(path, name))
    {
      printf("%s\n", path);
    }
    break;

  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
      if (de.inum == 0 || de.name[0] == '.')
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, name);
    }
    break;
  }
  close(fd);
}