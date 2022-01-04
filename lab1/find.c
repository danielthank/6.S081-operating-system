#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
find(char *folder, char *name, char *pattern)
{
  char file[100];
  strcpy(file, folder);
  strcpy(file + strlen(folder), "/");
  strcpy(file + strlen(folder) + 1 , name);
  if (file[strlen(file)-1] == '/') {
    file[strlen(file)-1] = 0;
  }

  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(file, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", file);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", file);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if (strcmp(name, pattern) == 0) {
      printf("%s/%s\n", folder, name);
    }
    break;

  case T_DIR:
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || de.name[0] == '.')
        continue;
      find(file, de.name, pattern);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{

  if(argc < 3){
    fprintf(2, "Usage: find <> <>\n");
  }

  find(argv[1], "", argv[2]);
  exit(0);
}
