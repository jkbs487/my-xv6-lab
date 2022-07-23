#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  if(argc != 1){
    fprintf(2, "usage: pingpong\n");
    exit(1);
  }

  int fds[2];
  if (pipe(fds) < 0) {
    printf("pipe error\n");
    exit(1);
  }
  int pid = fork();
  if (pid == 0) {
    char buffer[4];
    read(fds[0], buffer, 4);
    int parent_fd;
    memcpy(&parent_fd, buffer, 4);
    printf("%d: received ping\n", parent_fd);
    int child_fd = getpid();
    memcpy(buffer, &child_fd, 4);
    write(fds[1], buffer, 4);
    close(fds[0]);
    close(fds[1]);
  } else { 
    char buffer[4];
    int parent_fd = getpid();
    memcpy(buffer, &parent_fd, 4);
    write(fds[1], buffer, 4);
    read(fds[0], buffer, 4);
    int child_fd;
    memcpy(&child_fd, buffer, 4);
    printf("%d: received pong\n", child_fd);
    close(fds[0]);
    close(fds[1]);
  }

  exit(0);
}
