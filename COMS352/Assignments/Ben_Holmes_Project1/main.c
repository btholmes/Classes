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
int execute_multiple(char** args, int count); 

//This is an array of strings which represent the commands cd, exit, and history. 
char* functions_strings[] = {
  "cd", 
  "exit", 
  "history"
}; 
//Array of pointers to the functions represented by the commands cd, exit and history
int (*functions[]) (char **) = {
  &change_directory,
  &exit_loop, 
  &getHistory
};

//Boolean values to determine whether a valid command has been added to history, whether a semi colon was used in the command, and whether a pipe was used in the command. 
int successfulExecution = 0; 
int usedSemiColons = 0; 
int usedPipe = 0; 

//Char* array that represents the history
char** history;  
//Index for placing history elements in above array
int historyCount = 0; 

//Arrays for keeping track of the current, and previous directories. 
char previousDirectory[1024] = ""; 
char currentDirectory[1024] = ""; 

/**
Function handles the | command. 
**/
int runPipes(char **args, int argc){

  int parentCount = 0; 
  char **parentArgs = splitLine(args[0], &parentCount); 

  int childCount = 0; 
  char **childArgs = splitLine(args[1], &childCount); 

  int fd[2];
  pid_t pid;

  pipe(fd);

  pid = fork();

    if(pid==0)
    {
      dup2(fd[1], STDOUT_FILENO);
      close(fd[0]);
      close(fd[1]);
      execvp(parentArgs[0], parentArgs); 
      exit(1);
    }
    else
    { 
        pid=fork();

        if(pid==0)
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            close(fd[0]);
            execvp(childArgs[0], childArgs); 
            exit(1);
        }
        else
        {
            int status;
            close(fd[0]);
            close(fd[1]);
            waitpid(pid, &status, 0);
        }
    }
    return 1; 
}


/**
Reads in the input line from the command line
**/
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

char* removeQuotes(char* line){
  char *lineWithoutQuotes = malloc(strlen(line));
  int i, j;
  if(line[0] != '"')
      lineWithoutQuotes[0] = line[0];
  for(i = j = 1; i < strlen(line); i++){
      if(line[i] == '"' && line[i-1] != '\\')
          continue;
      lineWithoutQuotes[j++] = line[i];
  }
  lineWithoutQuotes[i] = '\0'; 
  return lineWithoutQuotes; 
}

/**
Splits the line read in from the command into a char** of args, with a NUll terminator.
 Also hanldes allocation of space. 
Handles splits for semi colons, spaces, and pipes. 
**/
char** splitLine(char *line, int *count){

   char* delimiter = " \t\r\n\a\0"; 
   usedSemiColons = 0; 
   usedPipe = 0; 

   char** tokens = malloc(sizeof(char*)*MAX_LINE); 
   line = removeQuotes(line); 

   int position = 0; 

   if(!tokens){
    fprintf(stderr, "Error allocatings space for tokens in split line"); 
    exit(EXIT_FAILURE); 
   }

    if(strchr(line, ';') != NULL){
        usedSemiColons = 1; 
        delimiter = ";\t\r\n\a\0"; 
    }else if(strchr(line, '|') != NULL){
      usedPipe = 1; 
      delimiter = "|\t\r\n\a\0"; 
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


/**
Function returns 0 back to main, which sets Should_run to 0 to break the loop. 
**/
int exit_loop(char** args){
  return 0; 
}

/**
allocates space for char** history if needed, stores current command at history[historyCount], increments historyCount. 
**/
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

/**
Prints out history to command line
**/
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

/**
Calls getcwd to store current directory as the new previous directory
**/
void setPreviousDirectory() {
    getcwd(previousDirectory, sizeof(previousDirectory));
}

/**
Similarly sets current directory after the call to chdir has finished successfully
**/
void setCurrentDirectory(){
  getcwd(currentDirectory, sizeof(currentDirectory)); 
}

/**
Uses chdir() command to change direcdtories, also sets previous and current directories to handle, "-" argument, 
as well as checks for existance of "~" in the arguments. 
**/
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

/**
Executes child process, and makes the parent wait for execution to finish. 
**/
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

/**
Executes parent and child concurrently without waiting. 
**/
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

/**
Called to handle "!!" and "!N" inputs, to execute command from char** history. 
**/
int executeHistory(int place){
  char* command = history[place]; 
  addToHistory(history, command); 
  int count = 0; 
  char** args = splitLine(command, &count); 

  // printf("history is %s %s %s\n", args[0], args[1], args[2]); 
  if(usedSemiColons)
    return execute_multiple(args, count); 
  else if(usedPipe)
    return runPipes(args, count); 

  return execute(args, count);
}

/**
Used to convert the N value from "!N" to an int
**/
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

/**
Function called immediately after a "!" argument is found in the command input. Handles error checking, then calls executeHistory. 
**/
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

/**
Handles execution of arguments for all calls, except in the case where they are separated by ";"
**/
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

/**
Handles execution of commands linked via ";"
**/
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
  char* line; //Line received from command line input
  char **args; //char **args represents the parsed, tokenized line
  int count = 0; //Count is the number of args

  history = malloc(sizeof(char*) * MAX_LINE);  //Array represeting all previous commands


  int should_run = 1; 

  while(should_run) {
    printf("osh> ");
    fflush(stdout); 

    line = getLine(); //Reads input

    addToHistory(history, line); //Adds to history

    args = splitLine(line, &count); //Splits line into tokens of arguments

    /**
    Below checks for semicolons or pipes, and handles accordingly
    **/
    if(usedSemiColons){
      should_run = execute_multiple(args, count); 
    }else if(usedPipe){
       // should_run = runPipes(args, count); 
          // printf("Count is %d \n", count); 
          // printf("%s \n %s \n", args[0], args[1]); 
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





