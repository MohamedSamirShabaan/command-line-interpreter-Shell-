#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#define MAX 1000


char input[MAX];
char *arg [MAX];
char *path [MAX];
char history[10][MAX];
char *valid_path;
int arg_index = 0;
int path_index = 0;
int history_index = 0;
int background = 0;

void run();
void historyHandle(char * get, char in[MAX]);
void batchMode(char const* const fileName);
void interactiveMode();
void intial();
int getCommand(char *input_Command);
void findPaths();
int concatenate();
void readHistory();
void saveHistory();
void printHistory();


void handle(int sig){
    pid_t pid;
    while((pid = waitpid(-1,NULL,WNOHANG)) > 0){
    }
    if(errno != ECHILD){printf("--->>>>> waitpid error\n");}
}

int main(int argc, char* const argv[])
{
    if(signal(SIGCHLD, handle)==SIG_ERR){printf("--->>> signal error!");}
    findPaths();
    readHistory();
    if(argc>1)
    {
        batchMode(argv[1]);
    }
    else
    {
        interactiveMode();
    }
    return 0;
}


// child and parent
void run()
{
    pid_t pid ;
    pid = fork();
    int status;
    if(pid < 0) printf("here pid < 0\n");
    if(pid == 0)
    {
        execv(valid_path, arg);
        printf("Error Path >>> Return from execv\n");
    }
    else
    {
        if(background == 0)
        {
            waitpid(pid, &status, 0);
        }
        else
        {

        }
    }
    return;
}


// read from file
void batchMode(char const* const fileName)
{
    FILE* file = fopen(fileName, "r");
    char line[MAX];

    if(!file)
    {
        printf("error file not find\n");
        exit(0);
    }
    while (fgets(line, sizeof(line), file))
    {
        intial();
        int d = history_index % 10;
        int ll = 0;
        char temp[MAX];
        for (int i = 0 ; i < 100 ; i++)
        {
            temp[i] = '\0';
            input[i] = '\0';
        }
        while(1)
        {
            if(line[ll] == '\0')
            {
                break;
            }
            ll++;
        }
        for(int i = 0 ; i < 150 ; i++)
        {
            temp[i] = '\0';
        }

        for(int i = 0 ; i < ll ; i++)
        {
            temp[i] = line[i];
        }
        int space = 0;
        for (int i = 0 ; i < strlen(line) ; i++)
        {
            if (line[i] != ' ' && line[i] != '\n' && line[i] != '\0')
            {
                space = 1;
                break;
            }
        }
        if (space == 0)
        {
            continue;
        }
        char *get = temp;
        if(strncmp(get, "\n", 1) == 0)
        {
            continue;
        }
        printf("---->>>>%s",line);
        int len = 0;
        if (strlen(line) > 81)
        {
            len = 1;
            printf("Error >>> Command greater than 80 character\n");
        }
        int here = 0;
        char ex[MAX] = "";
        for (int i = 0 ; i < ll ; i++)
        {
            if (line[i] != ' ' && line[i] != '\n')
            {
                ex[here++] = line[i];
            }
        }
        char *e = ex;

        if(strncmp(e, "exit", 4) == 0)
        {
            saveHistory();
            fclose(file);
            exit(0);

        }
        char ne = 'a';
        for (int i = 0 ; i < ll ; i++)
        {
            if (line[i] != ' ')
            {
                ne = line[i];
                break;
            }
        }

        if (ne == '!' && len == 0)
        {

            historyHandle(get, temp);
            continue;
        }
        for(int i = 0 ; i < 150 ; i++)
        {
            history[d][i] = '\0';
        }
        for(int i = 0 ; i < ll ; i++)
        {
            history[d][i] = line[i];
            input[i] = line[i];
        }


        history_index++;
        if (len == 1)continue;
        int his = getCommand(get);
        if (his == 1)continue;
        int flag = concatenate();
        if (flag)
        {
            run();
        }
        else
        {
            printf("Error >>> Command cannot be executed\n");
        }
    }
    saveHistory();
    fclose(file);
}

// read the history that stored
void readHistory()
{
    FILE* file = fopen("history.txt", "r"); /* should check the result */
    char line[MAX];

    if(!file)
    {
        return;
    }
    while (fgets(line, sizeof(line), file))
    {
        char *get = line;
        if(strncmp(get, "\n", 1) == 0)
        {
            continue;
        }
        for(int i = 0 ; i < strlen(line) ; i++)
        {
            history[history_index][i] = line[i];
        }
        history_index++;
    }

    fclose(file);
}


// run in interactive mode >> get command >> and check it then excuve
void interactiveMode()
{
    int should_run = 1;
    while(should_run)
    {
        intial();
        printf("shell>");
        char *get_input = NULL;
        get_input =  fgets(input, MAX, stdin);

        if (get_input == NULL)
        {
            saveHistory();
            exit(0);
        }
        if (strncmp(get_input, "\n", 1) == 0)
        {
            continue;
        }
        int len = 0;
        if (strlen(input) > 81)
        {
            len = 1;
            printf("Error >>> Command greater than 80 character\n");
        }

        int here = 0;
        char ex[MAX] = "";
        for (int i = 0 ; i < strlen(input) ; i++)
        {
            if (input[i] != ' ' && input[i] != '\n')
            {
                ex[here++] = input[i];
            }
        }
        char *e = ex;

        if(strncmp(e, "exit", 4) == 0)
        {
            saveHistory();
            exit(0);

        }
        char ne = 'a';
        for (int i = 0 ; i < strlen(input) ; i++)
        {
            if (input[i] != ' ')
            {
                ne = input[i];
                break;
            }
        }
        if (ne == '!' && len == 0)
        {
            historyHandle(get_input, input);
            continue;
        }
        int space = 0;
        for (int i = 0 ; i < strlen(input) ; i++)
        {
            if (input[i] != ' ' && input[i] != '\n' && input[i] != '\0')
            {
                space = 1;
                break;
            }
        }
        if (space == 0)
        {
            continue;
        }
        int d = history_index % 10;
        for(int i = 0 ; i < 100 ; i++)
        {
            history[d][i] = '\0';
        }
        for(int i = 0 ; i < strlen(input) ; i++)
        {
            history[d][i] = input[i];
        }
        history_index++;
        if (len == 1)continue;
        int his = getCommand(get_input);
        if (his == 1)continue;
        int flag = concatenate();
        if (flag)
        {
            run();
        }
        else
        {
            printf("Error >>> Command cannot be executed\n");
        }
    }
}

// to intial the arg & input array
void intial()
{
    for(int i = 0 ; i < arg_index ; i++)
    {
        arg[i] = NULL;
    }
    arg_index = 0;
    background = 0;
    valid_path = NULL;
    for (int i = 0; i < MAX ; i++)
    {
        input[i] = '\0';
    }
}

// to find the all paths in the begin of programm
void findPaths()
{
    char *path_string = getenv("PATH");
    char *pch ;
    pch = strtok(path_string, ":");
    while(pch != NULL)
    {
        path[path_index++] = pch;
        pch = strtok(NULL, ":");
    }
}

// to handle echo
void quote()
{
    int i = 0;
    while(i < strlen(input) - 1)
    {
        char temp[MAX];
        for (int k = 0;  k < 100 ; k++)temp[k] = '\0';
        int index = 0;
        if (input[i] != ' ' && input[i] != '\n' && input[i] != '"' && input[i] !='\'' && input[i] != '\t')
        {
            while(i < strlen(input) - 1 && input[i] != ' ' && input[i] != '\n' && input[i] != '"' &&
                    input[i] != '\'' && input[i] != '\t')
            {
                temp[index++] = input[i];
                i++;
            }
            char *s = temp;
            char * str3 = (char *) malloc(1 + strlen(s));
            strcpy(str3, s);
            arg[arg_index++] = str3;
        }
        index = 0;
        for (int k = 0; k < 100 ; k++)temp[k] ='\0';
        if (input[i] == '"' || input[i] == '\'')
        {
            i++;
            while(i < strlen(input) - 1 && input[i] != '\n' && input[i] != '\'' && input[i] != '"')
            {
                temp[index++] = input[i];
                i++;
            }
            char *ss = temp;
            char * str4 = (char *) malloc(1 + strlen(ss));
            strcpy(str4, ss);
            arg[arg_index++] = str4;
        }
        i++;
    }

    char *check = arg[arg_index - 1];
    arg[arg_index] = NULL;
    if (strncmp(check, "&", 1) == 0)
    {
        background = 1;
        arg[arg_index - 1] = NULL;
    }
}

// to parse and get command
int getCommand(char *input_Command)
{
    char *pch;
    char *check;

    for (int i = 0 ; i < strlen(input) ; i++)
    {
        if (input[i] == '"' || input[i] == '\'')
        {
            quote();
            return 0;
        }
    }

    pch = strtok(input_Command, " \n\t\r"); // " \n\t\r"
    while(pch != NULL)
    {
        arg[arg_index++] = pch;
        pch = strtok(NULL, " \n\t\r");
    }
    if (strncmp(arg[0], "history", 7) == 0 && arg_index == 1)
    {
        printHistory();
        return 1;
    }
    check = arg[arg_index - 1];
    arg[arg_index] = NULL;
    if (strncmp(check, "&", 1) == 0)
    {
        background = 1;
        arg[arg_index - 1] = NULL;
    }
    return 0;
}

// to concatenate the path + arg and check which one is valid
int concatenate()
{
    char *str2 ;
    str2 = "/";
    char *str = arg[0];

    for(int i = 0 ; i < path_index ; i++)
    {
        char *str1 = path[i];
        char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
        strcpy(str3, str1);
        strcat(str3, str2);
        char * str4 = (char *) malloc(1 + strlen(str3)+ strlen(str) );
        strcpy(str4, str3);
        strcat(str4, str);
        int flag = access(str4, F_OK);
        if(flag != -1)
        {
            valid_path = str4;
            return 1;
        }
    }
    int test = access(arg[0], F_OK);
    if(test != -1)
    {
        valid_path = arg[0];
        return 1;
    }
    return 0;
}

// save history from least to most
void saveHistory()
{
    FILE *fptr;
    fptr = fopen("history.txt", "w");
    if(fptr == NULL)
    {
        printf("Error! File Not Found\n");
        exit(1);
    }
    if(history_index <= 10)
    {
        for(int i = 0 ; i < history_index ; i++)
        {
            fprintf(fptr,"%s", history[i]);
        }
        fclose(fptr);
    }
    else
    {
        int i = 0;
        while(i <= 9)
        {
            int d = (i + history_index) % 10;
            fprintf(fptr,"%s", history[d]);
            i++;
        }
        fclose(fptr);
    }
}

// print history
void printHistory()
{
    if(history_index <= 10)
    {
        for(int i = 0 ; i < history_index ; i++)
        {
            printf("%d --> %s", i + 1 , history[i]);
        }
    }
    else
    {
        int i = 0 ;
        while(i <= 9)
        {
            int d = (i + history_index) % 10;
            printf("%d --> %s", i + 1, history[d]);
            i++;
        }
    }
}

// if command is !! or !N
void historyHandle(char *get, char in [MAX])
{
    int place = history_index % 10;
    int most = (history_index + 9) % 10;
    int target = 0;
    char temp [MAX];
    char *pch = strtok(get, " \n\t\r"); // " \n\t\r"
    in = pch;
    if (strncmp(pch, "!!", 2) == 0)
    {
        if(history_index == 0)
        {
            printf("error >>> No commands in history\n");
            return;
        }
        for(int i = 0 ; i < 150 ; i ++)
        {
            history[place][i] = '\0';
            temp[i] = '\0';
        }
        int l = 0;
        while(1)
        {
            if(history[most][l] == '\0')
            {
                break;
            }
            l++;
        }

        for(int i = 0; i < l ; i++)
        {
            history[place][i] = history[most][i];
            temp[i] = history[most][i];
        }

        int len = 0;
        if (strlen(temp) > 81)
        {
            len = 1;
            printf("Error >>> Command greater than 80 character\n");
        }
        for (int i = 0 ; i < l ; i++)input[i] = temp[i];
        char *s = temp;
        char * op = (char *) malloc(1 + strlen(s));
        strcpy(op, s);
        history_index++;
        if (len == 1)return;
        int his = getCommand(op);
        if (his == 1)return;
        int flag = concatenate();
        if (flag)
        {
            run();
        }
        else
        {
            printf("Error >>> Command cannot be executed\n");
        }
    }
    else if (in[0] =='!' && (strlen(in) == 2 || strlen(in) == 3))
    {
        int number = in[1]-'0';
        if (strlen(in) == 3)
        {
            number *= 10;
            number += (in[2]- '0');
        }
        if(number <= 0 || number > history_index || history_index == 0 || number > 10)
        {
            if (history_index == 0)
            {
                printf("error >>> No commands in history\n");
            }
            else
            {
                printf("error >>> No such command in history.\n");
            }
            return;
        }
        if(history_index <= 10)
        {
            target = number-1;
        }
        else
        {
            number--;
            target = (history_index + number) % 10;
        }

        for(int i = 0 ; i < 150 ; i ++)
        {
            if (target != place)
            {
                history[place][i] = '\0';
            }
            temp[i] ='\0';
        }

        int ll = 0;
        while(1)
        {
            if(history[target][ll] == '\0')
            {
                break;
            }
            ll++;
        }

        for(int i = 0; i < ll ; i++)
        {
            history[place][i] = history[target][i];
            temp[i] = history[target][i];
        }

        int len = 0;
        if (strlen(temp) > 81)
        {
            len = 1;
            printf("Error >>> Command greater than 80 character\n");
        }
        for (int i = 0 ; i < ll ; i++)input[i] = temp[i];
        char *ss = temp;
        char * hops = (char *) malloc(1 + strlen(ss));
        strcpy(hops, ss);
        history_index++;
        if (len == 1)return;
        int his = getCommand(hops);
        if (his == 1)return ;
        int flag = concatenate();
        if (flag)
        {
            run();
        }
        else
        {
            printf("Error >>> Command cannot be executed\n");
        }
    }
    else
    {
        printf("error Command to access history\n");
    }
}


