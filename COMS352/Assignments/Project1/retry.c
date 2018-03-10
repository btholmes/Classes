#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 80

int main(void)
{
  char *args[MAX_LINE/2 + 1];
  char ss[MAX_LINE];
  int should_run = 1;
  int i = 0;
  int j;
  while(should_run)
  {
        printf("OSH> ");
        fflush(stdout);
        scanf ("%[^\n]%*c", ss);
        printf("input:%s\n",ss);
        i = 0;
        int j;
        args[i] = strtok(ss," ");
        while (args[i] != NULL) {
          i++;
          args[i] = strtok(NULL, " ");
        }

        if(strcmp(args[0], "exit") == 0)
          break;
        if(strcmp(args[i-1], "&") != 0)
        {
          pid_t pid;
          pid = fork();
          if(pid < 0)
          {
                fprintf(stderr,"FORK Failed\n");
                return 1;
          }
          else if (pid == 0)
          {
                execvp(args[0],args);
                for(int j=0;j<i;j++)
                  args[j] = NULL;
          }
          else
          {
                wait(NULL);
          }
        }
        else
        {
          pid_t pid;
          pid = fork();
          if(pid < 0){
                fprintf(stderr,"FORK Failed\n");
                return 1;
          }
          else if (pid == 0){
                args[i-1] = NULL;
                execvp(args[0],args);
          }
          else
          {
                printf("\n\n");
          }
        }

  }
  return 0;
}