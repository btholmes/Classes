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