Ben Holmes
COMS 352
Assignment 4


1.) Terminal output from Linux 
PROGRAM 1

1927, 3060.   //  First iteration of for loop in parent process, here i == 0
3060, 3061    //  Duplicate of above parent process, this is the first child created
1927, 3060    //  Final iteration of for loop in original parent, here i == 1
3060, 3062    //  Child process, created by parent 3060
3060, 3061    //  Final iteration of original duplication of original parent process   
3061, 3063    //  Child of parent process 3061



PROGRAM 2
Here each for loop iteration creates a child process, and then they exit the for loop, and each print the statement. 

1927, 3129	  // First iteration of for loop, original parent is 3129. 
3129, 3131.   // This is the child created by first fork()
3129, 3130.   // Second iteration of for loop, child process 3130 is duplicated by fork. 
3130, 3132.   // Final child process



3.26.) Lecture 5 page 6

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
   const char *phrase = "Hi There";
   const int size = 1024; 
   char buf[size];
   int fd[2];
   int sd[2];
   pipe(fd);
   pipe(sd);

   //If parent process
   if (fork() != 0)
   {
     printf("%s\n", phrase);
     int writeSuccess = write(fd[1], phrase, strlen(phrase));
     int readSuccess = read(sd[0], buf, size);
     if(writeSuccess && readSuccess){
        printf("%s\n", buf);
     }else{
      printf("%s\n", "There was an error with either the write or read process"); 
     }
     
   }
   //Else child process
   else
   {
       // child
       read(fd[0], buf, size);
       int i;
       for (i = 0; buf[i]; i++)
       {
           if (isupper(buf[i]))
           {
               buf[i] = tolower(buf[i]);
           }
           else
           {
               buf[i] = toupper(buf[i]);
           }
       }
       write(sd[1], buf, strlen(buf));
   }
}


6.16) on Attached Page


6.19) 
	B. Shortest Job First


6.22) 
The amount of CPU time allocated to User processes could be maximized by switching to User processes before a given time quantum has fully completed. This would increase the priority associated with the process. 


6.24) 
(a) FCFS - No benefit or penalty accrues to short processes
(b) RR - Short processes will finish before longer processes





