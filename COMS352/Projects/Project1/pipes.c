#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

int change_directory(char **args); 
char* args[80]; 

int main (int argc, char ** argv) {
    // int i;

    // for( i=1; i<argc-1; i++)
    // {
    //     int pd[2];
    //     pipe(pd);

    //     if (!fork()) {
    //         dup2(pd[1], 1); // remap output back to parent
    //         execlp(argv[i], argv[i], NULL);
    //         perror("exec");
    //         abort();
    //     }

    //     // remap output from previous child to input
    //     dup2(pd[0], 0);
    //     close(pd[1]);
    // }

    // execlp(argv[i], argv[i], NULL);
    // perror("exec");
    // abort();

    // execlp("ls", "ls", NULL); 

    
    execlp("who", "who", NULL); 
}

int change_directory(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "change_directory: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("change_directory");
    }
  }
  return 1;
}