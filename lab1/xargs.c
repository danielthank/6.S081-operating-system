#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc <= 1){
    fprintf(2, "usage: xargs [command]\n");
    exit(1);
  }

  char child_buffer[MAXARG][32];
  char* child_argv[MAXARG];
  for (int i=0; i<argc-1; i++) {
    child_argv[i] = child_buffer[i];
    strcpy(child_buffer[i], argv[i+1]);
  }
  child_argv[argc-1] = child_buffer[argc-1];
  child_argv[argc] = 0;

  char *buffer = child_buffer[argc-1];
  while (gets(buffer, 32)){
    if (buffer[0] == 0) break;
    buffer[strlen(buffer)-1] = 0;
    int pid = fork();
    if (pid) {
      // parent
      wait(0);
      continue;
    }
    // child
    exec(child_argv[0], child_argv);
    fprintf(2, "exec error\n");
    exit(-1);
  }
  exit(0);
}