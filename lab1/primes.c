#include "kernel/types.h"
#include "user/user.h"

void
panic(char *s) {
  fprintf(2, "%s\n", s);
  exit(1);
}

void
process(int fd_read) {
  int first, i;
  read(fd_read, &first, 4);
  fprintf(1, "prime: %d\n", first);
  int p[2];
  int pipe_flag = 0;
  while (read(fd_read, &i, 4)) {
    if (i % first == 0) continue;
    if (!pipe_flag) {
      if (pipe(p) < 0) {
        panic("pipe");
      }
      pipe_flag = 1;
      int pid = fork();
      if (pid == 0) {
        // child
        close(p[1]);
        process(p[0]);
        close(p[0]);
      }
      close(p[0]);
    }
    write(p[1], &i, 4);
  }
  close(p[1]);
  wait(0);
  exit(0);
}

int
main(int argc, char *argv[]) {
  int p[2];
  if (pipe(p) < 0) {
    panic("pipe");
  }
  int pid = fork();
  if (pid == 0) {
    // child
    close(p[1]);
    process(p[0]);
    close(p[0]);
  }
  // parent
  close(p[0]);
  for (int i=2; i<=35; i++) {
    write(p[1], &i, 4);
  }
  close(p[1]);
  wait(0);
  return 0;
}
