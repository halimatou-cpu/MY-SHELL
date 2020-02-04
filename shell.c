#include <unistd.h>
#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

void affiche_cmd(char* argv[]) {
  for (int i = 0; i<100 && argv[i]!=NULL; i++) {
    printf("%s ", argv[i] );
  }
}

int main(int argc, char *argv[]) {
  affiche_cmd (argv) ;
  return 0;
}
