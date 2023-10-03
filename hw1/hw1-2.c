#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    char buffer[100];
    char *arguments[100];

    while (1)
    {
        printf("osh> ");
        fflush(stdout);
        int n = read(STDIN_FILENO, buffer, 100);
        buffer[n - 1] = '\0';   // replace '\n' with '\0' (for strtok)

        char *argument = strtok(buffer, " ");   // get the first argument
        int i = 0;
        while (argument != NULL)
        {
            arguments[i] = argument;
            argument = strtok(NULL, " ");   // get the next argument
            i++;
        }
        arguments[i] = NULL;    // add a nullptr at the end (for execvp)

        if (strcmp(arguments[0], "exit") == 0){  // exit command
            printf("Process end\n");
            return 0;
        }

        if (fork() == 0)    // child process
            execvp(arguments[0], arguments);
        else
            wait(NULL);
    }
    return 0;
}
