#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"



int
main(int argc, char *argv[])
{
  //if(argc != 3){
  //  printf("Usage: find PATH FILENAME\n");
  //  exit(0);
  //}

  char *args[MAXARG];
  int i = 0;
  for (i = 0; i < argc - 1; i++) {
    args[i] = argv[i+1];
  }

  char buffer[MAXARG];
  while (1) {
    char* s = gets(buffer, sizeof(buffer));
    int len = strlen(s);
    if (len == 0) break;
    args[i] = (char*) malloc(sizeof(char) * (len+1));
    strcpy(args[i], buffer);
    args[i][len-1] = 0;
    i++;
  }

  for (int j = 0; j < i; j++) {
    //printf("%s ", args[j]);
    //printf("%p\n", args[j]);
  }

  int pid = fork();
  if (pid == 0) {
    exec(args[0], args);
  }
  wait(0);
  exit(0);
}
