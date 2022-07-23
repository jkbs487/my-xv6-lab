#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *basename(char *path)
{
  int len = strlen(path);
  for (int i = len-1; i >= 0; i--) {
    if (path[i] == '/') {
      return &path[i+1];
    }
  }
  return path;
}

void
find(char *path, char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type) {

  case T_FILE:
    if (!strcmp(filename, basename(path)))
      printf("%s\n", path);
    break;

  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("find: path too long\n");
      break;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, filename);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf("Usage: find PATH FILENAME\n");
    exit(0);
  }
  char *path = argv[1];
  char *filename = argv[2];
  find(path, filename);
  exit(0);
}
