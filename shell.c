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
    //printf("%s", argv[i] );
    write(STDOUT_FILENO,argv[i],strlen(argv[i])+ 1);
    write(1, " ", 1);
  }
  write(1,"\n",1);
  //printf("\n" );
}
//Parseur qui découpe la chaine de caractere s
int parse_line (char * s, char ** argv[]){
  char* p = strdup(s);
  char* lu = strtok(p ," ");
  int i = 0;
  char* p1 = strpbrk(s,"#");
  if(p1){
    char* msg="Ceci est un commentaire:";
    write(1,msg,strlen(msg)+1);
    write(1,p1,strlen(p1)+1);
    write(1,"\n",1);
  }
   //printf("Ceci est un commentaire: %s\n",p1);
  while(lu != NULL){

    if(lu[0] != '#'){
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

void simple_cmd (char * argv[]){
  if (argv[0] == NULL) return;
  pid_t p;
  if(strcmp(argv[0],"exit")==0) exit(EXIT_SUCCESS);
  else if (strcmp(argv[0],"cd")==0){
    int change = chdir(argv[0]);
    if (change == -1) write(STDERR_FILENO,"Directory changement not done",BUFSIZ);
    // int fd= chdir(argv[0]);
    // if(fd == -1) return
  }else{
    p = fork();
    if(p == 0){
      execvp(argv[0],argv);
      return;
    }

  }
}

int main(int argc, char *argv[]) {
  char * s= "commande -v toto mammamia #ecrt  tata fatou";

  parse_line(s, (&argv) );
  affiche_cmd (argv) ;

  char reader[BUFSIZ];
  int lu;
  while( (lu=read(STDIN_FILENO,reader,BUFSIZ)) > 0){
  // while(1){
  //   lu=read(STDIN_FILENO,reader,BUFSIZ);
    //    printf("curdir$ %s\n", getcwd(argv[0],BUFSIZ) );
    char * curdir= getcwd(argv[0],BUFSIZ);
    write (1,curdir,strlen(curdir));
    write(1,"\n",1);
    //maintenant, on peut simplifier ton code, tjrs dans le parseur
    parse_line(reader, &argv);
    affiche_cmd (argv) ;
  //   printf("curdir$ %s\n", getcwd(argv,BUFSIZ) );
  //   //char * arg;
  //  //read (1)
  //  //scanf("%s\n",&arg );
  //   parse_line(argv[0],(&argv));
  //   affiche_cmd(argv);
  }

  return 0;
}


//Note à moi même, me debarrasser de tous mes printf, les
//remplacer par des read et write avant de rendre ma version finale sur le git
