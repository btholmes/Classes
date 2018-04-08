/**
Ben Holmes
COMS 352
Project 1
Spring 2018
**/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_LINE 80


int change_directory(char **args);
int exit_loop(char **args);
int getHistory(char **args);
int execute(char** args, int count); 
char** splitLine(char *line, int *count); 

char* functions_strings[] = {
  "cd", 
  "exit", 
  "history"
}; 

int (*functions[]) (char **) = {
  &change_directory,
  &exit_loop, 
  &getHistory
};

int usedSemiColons = 0; 
int usedPipe = 0; 

char** history;  
int historyCount = 0; 
int successfulExecution = 0; 

char previousDirectory[1024] = ""; 
char currentDirectory[1024] = ""; 

int runPipes(char ** argv, int argc) {
    int i;
    char **args; 

    for( i=0; i<argc - 1; i++)
    {
        int pd[2];
        pipe(pd);

        int argCount = 0; 
        args = splitLine(argv[i], &argCount); 

        if (fork() == 0) {
            close(STDOUT_FILENO); 
            close(pd[0]); 
            dup2(pd[1], 1); // remap output back to parent
            // execlp(argv[i], argv[i], NULL);
            if(execvp(args[0], args) < 0){
              perror("exec");
              abort();
              exit(EXIT_FAILURE); 
            } 
            
        }

        // if(fork() == 0){
        //     close(pd[1]); 
        //     dup2(pd[0], 0); 
        //     execvp(argv[argc - 1], arg); 
          
        // }
      close(STDIN_FILENO); 
      close(pd[1]); 
      dup2(pd[0], 0); 
      execlp(argv[argc -1], argv[argc-1], NULL); 
        // send output from child to input
    }

    return 1; 
}

// Function where the piped system commands is executed
int execArgsPiped(char** parsed, int count )
{
    int pipeArgCount = 0; 
    char** parsedpipe = splitLine(parsed[0], &pipeArgCount); 
    // 0 is read end, 1 is write end
    int pipefd[2]; 
    pid_t p1, p2, wpid;
    int status, status2; 
 
    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
    }
 
    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
 
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();
 
        if (p2 < 0) {
            printf("\nCould not fork");
        }
 
        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            // parent executing, waiting for two children
              do{
                wpid = waitpid(p1, &status, WUNTRACED);
             } while (!WIFEXITED(status) && !WIFSIGNALED(status));
             do{
                wpid = waitpid(p2, &status2, WUNTRACED); 
             } while (!WIFEXITED(status2) && !WIFSIGNALED(status2)); 
        }
    }
    return 1; 
}



char* getLine(void){
  int bufsize = 1024;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    if (position >= bufsize) {
      bufsize += 1024;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}


char** splitLine(char *line, int *count){

   char* delimiter = " \t\r\n\a"; 
   usedSemiColons = 0; 
   usedPipe = 0; 

   char** tokens = malloc(sizeof(char*)*MAX_LINE); 
   
   int position = 0; 

   if(!tokens){
    fprintf(stderr, "Error allocatings space for tokens in split line"); 
    exit(EXIT_FAILURE); 
   }

    if(strchr(line, ';') != NULL){
        usedSemiColons = 1; 
        delimiter = ";\t\r\n\a"; 
    }else if(strchr(line, '|') != NULL){
      usedPipe = 1; 
      delimiter = "|\t\r\n\a"; 
    }

   tokens[position] = strtok(line,delimiter);

    while (tokens[position] != NULL) {
      position++;

      if(position >= MAX_LINE){
        tokens = realloc(tokens, sizeof(char*) *MAX_LINE); 
        if(!tokens){
            fprintf(stderr, "allocation error\n");
            exit(EXIT_FAILURE);
        }
      }

      tokens[position] = strtok(NULL, delimiter);
    } 
    *count = position; 
    return tokens; 

}



int exit_loop(char** args){
  return 0; 
}

void addToHistory(char** history, char* line){
      if(historyCount >= MAX_LINE){
        history = realloc(history, sizeof(char*) * MAX_LINE); 
        if(!history){ 
          printf("Allocation error in add to history"); 
        }
      }
      history[historyCount] = malloc(sizeof(char) * MAX_LINE + 1); 
      strcpy(history[historyCount], line); 
 
      historyCount++; 
}


int getHistory(char** args){
    int i = historyCount-1; 
    int j = 10; 
    if(historyCount < 10){
        j = historyCount; 
        while(j > 0){
          printf("%d  %s\n", j, history[i]);
          i--; 
          j--; 
        }
    }else{
        while(j > 0){
          printf("%d. %s\n", j, history[i]);
          i--;  
          j--; 
        }
    }
    return 1; 
}

void setPreviousDirectory() {
    getcwd(previousDirectory, sizeof(previousDirectory));
}

void setCurrentDirectory(){
  getcwd(currentDirectory, sizeof(currentDirectory)); 
}


int change_directory(char **args)
{
  if (args[1] == NULL) {
      fprintf(stderr, "No argument supplied \"cd\"\n");
  }else if(strcmp(args[1], "-") == 0){

        if(strcmp(previousDirectory, "") == 0){
            printf("You haven't been to any other directories yet \n"); 
        }else{
            // setCurrentDirectory(); 
            char previous[1024]; 
            strcpy(previous, previousDirectory); 
            setPreviousDirectory(); 
            if (chdir(previous) == 0) {
              setCurrentDirectory(); 
            }
        }
      printf("\n"); 

  }else if(strcmp(args[1], "~") == 0){
     setPreviousDirectory(); 
     if (chdir(getenv("HOME")) == 0) {
          setCurrentDirectory(); 
      }
  }else{
    setPreviousDirectory(); 
     if (chdir(args[1]) == 0) {
          setCurrentDirectory(); 
      }
  }
  return 1;
}


int makeParentWait(char** args){
    pid_t pid, wpid;
    pid = fork();
    int status; 

    if(pid < 0)
    {
      perror("fork"); 
    }
    else if (pid == 0)
    {
       if(execvp(args[0],args) == -1){
          perror("Executing"); 
          exit(EXIT_FAILURE); 
       }       
       exit(EXIT_SUCCESS); 
    }
    else
    {
      do {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1; 
}

int executeConcurrently(char** args, int count){
    int i; 
    pid_t pid;
    pid = fork();
    if(pid < 0){
        perror("fork"); 
        exit(EXIT_FAILURE); 
    }
    else if (pid == 0){
        args[count -1] = NULL; 

        if(execvp(args[0],args) == -1){
          perror("Executing"); 
          exit(EXIT_FAILURE); 
        }   
        exit(EXIT_SUCCESS); 
    }
    else
    {
        // exit(EXIT_SUCCESS); 
        // printf("\n\n");
    }
    return 1; 
}

int executeHistory(int place){
  char* command = history[place]; 
  addToHistory(history, command); 
  int count = 0; 
  char** args = splitLine(command, &count); 

  return execute(args, count);
}

int toint(char str[])
{

  if(str[1] == '!'){
    return 33; 
  }

  int len = strlen(str);
  int i, num = 0;

  for (i = 1; i < len; i++)
  {  
      num = num + ((str[len - (i )] - '0') * pow(10, i-1));
  }
 
   return num;
}

int handleBang(char **args, int count){
      int n = toint(args[0]); 

      if(n == 33){
          if(historyCount < 2 || !successfulExecution){
            fprintf(stderr, "No commands in history buffer \n");
            return 1; 
          }
          return executeHistory(historyCount -2); 
      }else{
          if(historyCount <= 10 && n <= historyCount){
            return executeHistory(n-1); 
          }else{

            int position = n; 
            if(n >= historyCount){
              fprintf(stderr, "That N value is too large \n"); 
              return 1; 
            }
            return executeHistory(historyCount - 12 + (n)); 
          }
      }
}


int execute(char** args, int count){
  if(args[0] == NULL){
    return 1; 
  }
   int i; 
   for(i = 0; i < sizeof(functions_strings)/sizeof(char*); i++){
      if(strcmp(args[0], functions_strings[i]) == 0){
        successfulExecution = 1; 
        return (*functions[i])(args); 
      }
    }

  if(args[0][0] == '!'){
    return handleBang(args, count); 
  }

  if(strcmp(args[count-1] , "&") == 0){
    successfulExecution = 1; 
    return executeConcurrently(args, count); 
  }
  successfulExecution = 1; 
  return makeParentWait(args); 
}

int execute_multiple(char **args, int count){
  int i = 0; 
  char **arglist; 
  while(i < count){
    int thisCount = 0; 
    arglist = splitLine(args[i], &thisCount); 
    execute(arglist, thisCount); 
    i++; 
  }

  return 1; 
}


int main(int argc, char **argv)
{
  char* line; 
  char **args;
  int count = 0; 

  history = malloc(sizeof(char*) * MAX_LINE); 


  int should_run = 1; 

  while(should_run) {
    printf("osh> ");
    fflush(stdout); 

    line = getLine(); 

    addToHistory(history, line); 

    args = splitLine(line, &count); 

    if(usedSemiColons){
      should_run = execute_multiple(args, count); 
    }else if(usedPipe){
       should_run = runPipes(args, count); 
       // should_run = execArgsPiped(args, count); 
    }else{
      should_run = execute(args, count); 
    }

    free(line);
    free(args);
  } 

  free(history); 

  return EXIT_SUCCESS;
}





