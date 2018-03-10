/**
 Iowa State University
 Fall 2017
 Ram Luitel
 ComS352
 Project 1

**/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_HISTORY 50
#define HISTORY "\nShell Command History:\n"

char history[10][MAX_HISTORY];
int count=0;


void viewHistory()
{
    printf(HISTORY);
    int i;
    int j = 0;
    int histCount = count;
    //iteration to display the history
    for (i = 0; i<10;i++)
    {

        printf("%d.  ", histCount);
        while (history[i][j] != '\n' && history[i][j] != '\0') //is it the last element in the array?
        {
            printf("%c", history[i][j]);
            j++;
        }

        printf("\n");
        j = 0;
        histCount--;
        if (histCount ==  0)
            break;
    }
    printf("\n");
}

int processCommand(char inputBuffer[], char *args[],int *flag)
{
   	  int length; // no. of characters in command line
    	int i;     // loop index for inputBuffer
    	int start;  // index of beginning of next command
    	int ct = 0; // index of where to place the next parameter into args[]
    	int hist;
    	//read user input on command line and checking whether the command is !! or !n

    	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
	    start = -1;
	    if (length == 0)
	        exit(0);   //end of command
	    if (length < 0)
	    {
	        printf("Command not read\n");
	        exit(-1);  //terminate
	    }
	   //examine each character
	    for (i=0;i<length;i++)
	    {
	        switch (inputBuffer[i])
	        {
	            case ' ':
	            case '\t' :               //separatng arguments
	                if(start != -1)
	                {
	                    args[ct] = &inputBuffer[start];
	                    ct++;
	                }
	                inputBuffer[i] = '\0'; // add a null char at the end
	                start = -1;
	                break;

	            case '\n':
	                if (start != -1)
	                {
	                    args[ct] = &inputBuffer[start];
	                    ct++;
	                }
	                inputBuffer[i] = '\0';
	                args[ct] = NULL;
	                break;

	            default :
	                if (start == -1)
	                    start = i;
	                if (inputBuffer[i] == '&')
	                {
	                    *flag  = 1;
	                    inputBuffer[i] = '\0';
	                }
	              }
	   	 }

	    	args[ct] = NULL; //if the input line was > 80

		if(strcmp(args[0],"history")==0)
	  {
	     if(count>0)
		      viewHistory();
			else
			   printf("\nNo Commands in the history\n");

			return -1;
	   }
		else if (args[0][0]-'!' ==0)
		{
	    int x = args[0][1]- '0';
			int z = args[0][2]- '0';
			if(x>count) //second letter check
			{
			     printf("\nNo Such Command in the history\n");
			     strcpy(inputBuffer,"Wrong command");
			}
			else if (z!=-48) //third letter check
			{
			    printf("\nNo Such Command in the history. Enter <=!9 (buffer size is 10 along with current command)\n");
			    strcpy(inputBuffer,"Wrong command");
			}
			else
			{
	       if(x==-15)//Checking for '!!',ascii value of '!' is 33.
				 {
	          strcpy(inputBuffer,history[0]);  // this will be your 10 th(last) command
				}
				else if(x==0) //Checking for '!0'
				{
	        printf("Enter proper command");
					strcpy(inputBuffer,"Wrong command");
				}
				else if(x>=1) //Checking for '!n', n >=1
				{
					strcpy(inputBuffer,history[count-x]);
				}

			}
		}
	      for (i = 9;i>0; i--) //Moving the history elements one step higher
	       	strcpy(history[i], history[i-1]);
	        strcpy(history[0],inputBuffer); //Updating the history array with input buffer
	        count++;
		   if(count>10)
		      count=10;
 }
/**
  Main function
**/
int main()
{
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int run = 1; /* flag to determine when to exit program */
    int flag;
    char commandBuffer[MAX_LINE]; //current command string
    pid_t pid,tpid;
    int i;
    while (run) {
    printf("osh>");
    fflush(stdout);
     if(-1!=processCommand(commandBuffer,args,&flag)) // get next command
	    {
		      pid = fork();
        	if (pid < 0)//if pid is less than 0, forking fails
        	{
            		printf("Fork failed.\n");
            		exit (1);
        	}
       		 else if (pid == 0)//if pid ==0
        	{
           	 	//command not executed
            		if (execvp(args[0], args) == -1)
           	 	{
                		printf("Error executing command\n");
            		}
       		 }

       		 // if flag == 0, the parent will wait,
        	// otherwise returns to the processCommand() function.
        	else
        	{
        		 i++;
           	  if (flag == 0)
              {
                	i++;
                	wait(NULL);
           	 }
        	}
   	 }
    }
    return 0;
}
