// fuck ye IBM https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.bpxbd00/rtoped.htm

/* CELEBO01

   This example opens a directory.

 */
#define _POSIX_SOURCE
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <iostream>
#include <cstring>
using namespace std;

void traverse(char *fn, int indent) {
  DIR *dir;
  struct dirent *entry;
  int count;
  char path[1025];
  struct stat info;

  for (count=0; count<indent; count++) printf("  ");
  printf("%s\n", fn);

  if ((dir = opendir(fn)) == NULL)
    perror("opendir() error");
  else {
    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_name[0] != '.') {
          cout << "path: " << path << "\n";
        strcpy(path, fn);
	cout << "strcpy path fn: " << path << "\n";
        strcat(path, "/");
	cout << "strcpy path /:" << path << "\n";
        strcat(path, entry->d_name);
	cout << "strcpy path entry-d_name: " << path << "\n";
        if (stat(path, &info) != 0)
          fprintf(stderr, "stat() error on %s: %s\n", path,
                  strerror(errno));
        else if (S_ISDIR(info.st_mode))
               traverse(path, indent+1);
      }
    }
    closedir(dir);
  }
}

main() {
  puts("Directory structure:");
  traverse("/etc", 0);
}
