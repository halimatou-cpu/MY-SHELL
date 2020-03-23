#include <unistd.h>
#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

void affiche_cmd(char* argv[]) {
  for (int i = 0; i<100 && argv[i]!=NULL; i++) {
    printf("%s", argv[i] );
  }
  printf("\n" );
}
//Parseur qui découpe la chaine de caractere s
int parse_line (char * s, char ** argv[]){
  char *p = strdup(s);
  char * lu = strtok(p ," ");
  int i =0;
   char* p1=strpbrk(s,"#");
   if(p1) printf("Ceci est un commentaire: %s\n",p1);
  while(lu != NULL){
    // if(strcmp(lu[0],p1[0])!=0){
      if(lu[0]!=p1[0]){
      (*argv)[i]=malloc(sizeof(char)*(1+ strlen(lu)));
      (*argv)[i]=lu;
      lu = strtok(NULL," ");

      i++ ;
    }else{
      (*argv)[i]=NULL;
      return 0;
    }
  }
  (*argv)[i]=NULL;

  return 0;
}



int main(int argc, char *argv[]) {
  //char * argvv = argv;
  char * s= "commande -v toto mammamia #ecrt  tata fatou";

  parse_line(s, (&argv) );
  affiche_cmd (argv) ;

  // char arg[BUFSIZ];
  // char* in;
  // char* out;

  while(1){
    printf("curdir$ %s\n", getcwd(argv[0],BUFSIZ) );
    char reader[BUFSIZ];
    read(0,reader,BUFSIZ);
    parse_line(reader, &argv);
    affiche_cmd (argv);
  //   printf("curdir$ %s\n", getcwd(argv,BUFSIZ) );
  //   //char * arg;
  //   //read (1)
  // //  scanf("%s\n",&arg );
  //   parse_line(argv[0],(&argv));
  //   affiche_cmd(argv);
  }

  return 0;
}


//Note à moi même, me debarrasser de tous mes printf, les
//remplacer par des read et write avant de rendre ma version finale sur le git
