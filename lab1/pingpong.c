#include "kernel/types.h"
#include "user/user.h"

void
panic(char *s) {
  fprintf(2, "%s\n", s);
  exit(1);
}

int
main(int argc, char *argv[]) {
  int p1[2], p2[2];
  if (pipe(p1) < 0) {
    panic("pipe");
  }
  if (pipe(p2) < 0) {
    panic("pipe");
  }
  int pid = fork();
  if (pid < 0) {
    panic("fork");
  }
  if (pid == 0) {
    // child
    pid = getpid();
    while (1) {
      read(p1[0], (void*) 0, 1);
      fprintf(1, "%d: received ping\n", pid);
      write(p2[1], (void*) 'a', 1);
    }
  } else {
    // parent
    pid = getpid();
    while (1) {
      write(p1[1], (void*) 'a', 1);
      read(p2[0], (void*) 0, 1);
      fprintf(1, "%d: received pong\n", pid);
    }
  }
}
