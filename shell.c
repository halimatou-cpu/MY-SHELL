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
    printf("%s ", argv[i] );
  }
  printf("\n" );
}

// int parse_line ( char *s, char** argv[]){
//   char* cut ;
//   int i =0;
//   cut = strtok (s," ");
//   (*argv)[i] = cut;
//   i++;
//   while (cut != NULL /*&& cut != '\0'*/){
//     cut = strtok(NULL," ");
//     //cut = strtok (s," ");
//     (*argv)[i] = cut;
//     i++;
//   }
//   (*argv)[i] = NULL;
//   return 0;
// }

// int parse_line (char * s, char ** argv[]){
//   char * lu = strpbrk (s ," ");
//   int i =0;
//   while(lu != NULL && s[i] != '\0'){
//       if(strcmp(lu," ")!=0){
//       (*argv)[i]=lu;
//       lu = strpbrk(s," ");
//       i++ ;
//     }else lu = strpbrk(s," ");
//   }
//   (*argv)[i]=NULL;
//   return 0;
// }
//
// int parse_line_ (char * s, char * argv[]){
//   char *p = strdup(s);
//   char * lu = strpbrk (p ," ");
//   int i =0;
//   while(lu != NULL && s[i+1] != '\0'){
//     if(strcmp(lu," ")!=0){
//       argv[i]=malloc(sizeof(char)*(1+ strlen(lu)));
//       strcpy(argv[i],lu);
//       lu = strpbrk(s," ");
//       i++ ;
//     }else lu = strpbrk(s," ");
//   }
//   argv[i]=NULL;
//   return 0;
// }
//
//
// int main(int argc, char *argv[]) {
//   //char * argvv = argv;
//   char * s= "commande -v toto   tata";
//   (*argv) = malloc (sizeof(s) +1);
//   parse_line(s, &(argv) );
//   affiche_cmd (argv) ;
//   return 0;
// }


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

  while(1){
    printf("curdir$ %s\n", getcwd(argv,BUFSIZ) );
    char * arg;
    //read (1)
  //  scanf("%s\n",&arg );
    parse_line(arg,(&argv));
    affiche_cmd(argv);
  }

  return 0;
}


//Note à moi même, me debarrasser de tous mes printf, les
//remplacer par des read et write avant de rendre ma version finale sur le git
