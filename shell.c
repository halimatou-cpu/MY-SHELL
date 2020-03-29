#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <error.h>
#include <sys/wait.h>

#define BUFSIZE 1024
#define MAX_ARG 100

//Fonction qui affiche tous les elements d'un tableau
void affiche_cmd(char *argv[])
{
    for (int i = 0; i < 100 && argv[i] != NULL; i++)
    {
        write(STDOUT_FILENO, argv[i], strlen(argv[i]));
        write(1, " ", 1);
    }
    write(1, "\n", 1);
}

//fonction qui recupere une partie d'une chaine de car jusqu'a l'indice i
char *sub_string(const char *s, int i)
{
    char *cont = (char *)malloc(sizeof(char) * (i + 1));

    for (unsigned int j = 0; j < strlen(s) && j < (unsigned int)i; j++)
    {
        cont[j] = s[j];
    }
    cont[i] = '\0';
    return cont;
}

//Parse en fonction des espaces (question 5)
int parse_line(char *s, char **argv[])
{

    char *p1 = strpbrk(s, "#");
    int len = p1 == NULL ? 0 : strlen(p1);
    int t = strlen(s) - len;

    char *p = sub_string(s, t);
    const char *sep = " ";
    char *lu = strtok(p, sep);

    int i = 0;
    while (lu)
    {
        argv[i] = (char **)malloc(sizeof(char) * (1 + strlen(lu)));
        (*argv)[i] = lu;
        lu = strtok(NULL, " ");
        i++;
    }
    (*argv)[i] = NULL;
    return i;
}

//Parseur qui ne prend pas de tableau mais en renvoi un apres avoir parsé une chaine
char **parse_line_bis(const char *cmd)
{
    char **result = malloc(MAX_ARG * (sizeof(char *)));

    char *p1 = strpbrk(cmd, "#");
    int len = p1 == NULL ? 0 : strlen(p1);
    int t = strlen(cmd) - len;

    char *p = sub_string(cmd, t);
    char *lu = strtok(p, " ");

    int i = 0;
    while (lu)
    {
        result[i] = (char *)malloc(sizeof(char) * (1 + strlen(lu)));
        strcpy(result[i], lu);
        lu = strtok(NULL, " ");
        i++;
    }
    result[i] = NULL;

    return result;
}

// une procedure qui libere tous les mallocs char**
void free_2D(char **array)
{
    int i = 0;
    while (array[i])
        free(array[i++]);
}

/*Fonction qui crée un processus fils qui exécute une cmd
void executor(char *command[])
{
    pid_t pid = fork();

    if (pid > 0)
    {
        wait(NULL);
    }
    else if (!pid)
    {
        execvp(command[0], command);
        write(STDERR_FILENO, "Erreur dans execvp\n", 20);
    }
    else
    {
        // error
    }
}
*/

//fonction qui nettoie un buffer
void cleaner(char *buffer)
{
    int i;
    for (i = 0; i < BUFSIZE; i++)
        buffer[i] = '\0';
}

void simple_cmd(char *argv[]);

void cmd_executor()
{
    int tmp;
    char INPUT[BUFSIZE];
    char **result;

    while (1)
    {
        cleaner(INPUT);

        tmp = read(STDIN_FILENO, INPUT, BUFSIZE);

        if (tmp == -1)
        {
            write(STDERR_FILENO, "non valid\n", 10);
            exit(-1);
        }

        INPUT[tmp - 1] = '\0';

        result = parse_line_bis(INPUT);

        simple_cmd(result);

        // executor(result);
        free_2D(result);
    }
}

void simple_cmd(char *argv[])
{
    if (argv[0] == NULL)
        return;
    pid_t p;
    if (strcmp(argv[0], "exit") == 0)
        exit(EXIT_SUCCESS);
    else if (strcmp(argv[0], "cd") == 0)
    {
        if (argv[1] != NULL)
        {
            write(1, argv[1], strlen(argv[1]));
            write(1, "\n", 1);
            chdir(argv[1]);
        }
        else
        {
            char parse[BUFSIZE];
            getcwd(parse, BUFSIZE);
            unsigned int len = strlen(parse);
            int count = 0;
            unsigned int i;
            for (i = 0; i < len && count < 3; i++)
            {
                if (parse[i] == '/')
                    count++;
            }
            write(1, sub_string(parse, i), strlen(sub_string(parse, i)));
            chdir(sub_string(parse, i));
        }
    }
    else
    {
        p = fork();
        if (p == 0)
        {
            execvp(argv[0], argv);
            wait(0);
        }
    }
}

//Retourne l'indice de la premiere occurence du caractere 'c'
//dans la chaîne de caractère s
int find_char(char *s, char c)
{
    int len = strlen(s);
    int i = 0;
    for (i = 0; i < len; i++)
    {
        if (s[i] == c)
            return i;
    }
    return -1;
}

void parse_line_redir(char *s, char **argv[], char **in, char **out)
{

    parse_line(s, argv);
    int i = 1;
    int len = 0;
    while ((*argv)[i])
    {
        if ((find_char((*argv)[i], '>')) != -1)
        {
            in[len] = (char *)malloc(sizeof(char) * (1 + strlen((*argv)[i - 1])));
            in[len] = (*argv)[i - 1];
            if ((*argv)[i + 1])
            {
                out[len] = (char *)malloc(sizeof(char) * (1 + strlen((*argv)[i + 1])));
                out[len] = (*argv)[i + 1];
            }
            else
                out[len] = NULL;
            len++;
        }
        if ((find_char((*argv)[i], '<')) != -1)
        {
            out[len] = (char *)malloc(sizeof(char) * (1 + strlen((*argv)[i - 1])));
            out[len] = (*argv)[i - 1];
            if ((*argv)[i + 1])
            {
                in[len] = (char *)malloc(sizeof(char) * (1 + strlen((*argv)[i + 1])));
                in[len] = (*argv)[i + 1];
            }
            else
                in[len] = NULL;
            len++;
        }
        i++;
    }
}

int main(int argc, char *argv[])
{
    write(1, argv[argc], 0);
    // affiche_cmd(parse_line_bis("voyons cette commande # apres parse"));

    //l'appel de cmd_executor dans le main contient la question 6
    //avec en plus l'exécution des commandes une fois que l'utilisateur
    //tape sur entrée
    cmd_executor();
    return 0;
}