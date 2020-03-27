#include <unistd.h>
#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

void affiche_cmd(char* argv[]) {
  for (int i = 0; i<100 && argv[i]!=NULL; i++) {
    write(STDOUT_FILENO, argv[i], strlen(argv[i]));
    write(1, " ", 1);
  }
  write(1,"\n",1);
}

char * sub_string (char * s, int i){
  char* cont = (char*) malloc(sizeof(char)*(i+1));

  for(unsigned int j=0; j<strlen(s) && j<(unsigned int)i ;j++){
    cont[j]=s[j];
  }
  cont[i]='\0';
  return cont;
}

//Retourne l'indice de la premiere occurence du caractere 'c'
//dans la chaîne de caractère s
int find_char(char* s, char c){
  int len = strlen(s);
  int i = 0;
  for (i = 0; i < len; i++){
    if(s[i] == c) return i;
  }
  return -1;
}

//Parseur qui découpe la chaine de caractere s
int parse_line (char * s, char ** argv[]){

  char* p1 = strpbrk(s,"#");
  int len = p1 == NULL? 0 : strlen(p1);
  int t = strlen(s) - len;

  char *p = sub_string(s,t);
  const char * sep = " ";
  char* lu = strtok(p , sep);

  int i = 0;
  while(lu){
    argv[i]= (char **) malloc(sizeof(char)*(1+ strlen(lu)));
    (*argv)[i]=lu;
    lu = strtok(NULL," ");
    i++ ;
  }
  (*argv)[i] = NULL;
  return i;
}

void test_parse(char *argv[]){
  char * s= "commande -v toto mammamia #ecrt  tata fatou";

  parse_line(s, (&argv) );
  affiche_cmd (argv) ;

  write(1, sub_string(s,4), strlen(sub_string(s,4)));
}

void simple_cmd (char * argv[]){
  if (argv[0] == NULL) return;
  pid_t p;
  if(strcmp(argv[0],"exit")==0) exit(EXIT_SUCCESS);
  else if (strcmp(argv[0],"cd")==0){
    // int change;
    if(argv[1]!=NULL){
      write(1, argv[1], strlen(argv[1]));
      write(1,"\n",1);
      // change =
      chdir(argv[1]);
      //if (change == -1) write(STDERR_FILENO,"Directory changement not done",BUFSIZ);
    }else {
      char parse [BUFSIZ];
      getcwd (parse, BUFSIZ) ;
      unsigned int len = strlen (parse);
      int count=0;
      unsigned int i;
      for (i = 0; i < len && count <3 ; i++) {
        if(parse[i]=='/') count++ ;
      }
      write(1, sub_string(parse, i), strlen(sub_string(parse, i)));
      // change =
      chdir (sub_string(parse, i));
    }
  }else{
    p = fork();
    if(p == 0){
      execvp(argv[0],argv);
      wait(0);
    }
  }
}

void cmd_main(){
  // char reader[BUFSIZ];
  char * reader=(char*)malloc(sizeof(char *));
  int lu;

  char curdir[BUFSIZ];
  getcwd(curdir, BUFSIZ);
  write (1,curdir,strlen(curdir));
  write(1,"$ ",2);

  char **holder ;
  // char *holder[100];

  while( (lu=read(STDIN_FILENO,reader,BUFSIZ)) > 0){
    getcwd(curdir, BUFSIZ);

    write (1,curdir,strlen(curdir));
    write(1,"$ ",2);
    write(1,"\n",1);

    parse_line(reader, &holder);
    affiche_cmd (holder);
    simple_cmd (holder);
    // free(holder);
  }

}

void script_main(char *argv[]){
  if(argv[1]){
    int fd = open(argv[1], O_RDONLY | O_NONBLOCK, 0773);
    if(fd != -1){
      write(1,"voyons ",6);
      int lire;
      char ** tab;
      char buffer[BUFSIZ];
      while ((lire=read(fd,buffer,BUFSIZ))>0){
        write(1,"Rienderien",10);
        parse_line(buffer, &tab);
        affiche_cmd(tab);
        simple_cmd(tab);
        write(1,"wesh",4);
      }
      close(fd);
    }else{
       write(1,"script pas ouvert",17);
    }
  }
}

int parse_line_redir (char* s, char **argv[], char **in,char **out){

  parse_line(s,argv);
  int i = 1;
  int len=0;
  while ((*argv)[i]){
    if ( (find_char ((*argv)[i],'>') ) != -1 ){
      in[len] = (char *) malloc(sizeof(char)*(1+strlen((*argv)[i-1])));
      in[len] = (*argv)[i-1];
      if((*argv)[i+1]){
        out[len] = (char *) malloc(sizeof(char)*(1+strlen((*argv)[i+1])));
        out[len] = (*argv)[i+1];
      }else out[len] = NULL;
      len++;
    }
    if ( (find_char((*argv)[i],'<') ) != -1 ){
      out[len] = (char *) malloc(sizeof(char)*(1+strlen((*argv)[i-1])));
      out[len] = (*argv)[i-1];
      if((*argv)[i+1]){
        in[len] = (char *) malloc(sizeof(char)*(1+strlen((*argv)[i+1])));
        in[len] = (*argv)[i+1];
      }else in[len] = NULL;
      len++;
    }
    i++;
  }
  return 0;
}

//oui? Cool

int main(int argc, char *argv[]) {
  test_parse(argv);
  cmd_main();
  script_main(argv);
  write (1,argv[argc],0);
  // char ** tab = (char **)malloc(sizeof(char*));
  // char ** tab[100] ;
  // char ** out = (char **) malloc(sizeof(char*));
  // char ** in = (char **) malloc(sizeof(char*));
  // parse_line_redir ("test < hali momo voir > alors",tab,in,out);

  // write(1,"\n",1);
  // write(1,"le contenu de out: ",19);
  // affiche_cmd(out);
  // write(1,"\n",1);
  // write(1,"contenu de in: ",15);
  // affiche_cmd(in);

  // free (tab);
  // free (out);
  // free (in);

  return 0;
}

//un fork pour toutes les fonc _cmd

//Note à moi même, me debarrasser de tous mes printf, les
//remplacer par des read et write avant de rendre ma version finale sur le git
