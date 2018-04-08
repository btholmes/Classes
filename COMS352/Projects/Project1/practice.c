#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* function declaration */
double getAverage(int* arr, int size);
char cwd[1024] = ""; 
int count = 0; 
// char *history[11]; 
char **history; 


void getDirectory(){


  // if(getcwd(cwd, sizeof(cwd)) != NULL){
  //     printf("%s\n", cwd); 

  // }
  getcwd(cwd, sizeof(cwd)); 
}

void stringPlay(char *a, char **b, char**c){
  a = "Hello"; 
  printf("%s \n", a); 
  *b = "World"; 
  *c = "Did it work"; 
}
 
void arrayPlay(char **array){
  array[0] = "Hello"; 
  array[0] = "New"; 
  array[1] = "world"; 
  array[2] = "Third"; 
  array[3] = "Fourst"; 
}
void printHistory(void){
  int i; 
  for(i = count-1; i >= 0; i-- ){
    printf("%d : %s\n", i, history[i]);  
  }
}

int main () {

  // char* line; 
  // history = malloc(sizeof(char*) * 20); 
  // // while(count < 10){
  //   // line = malloc(sizeof(char) * 81); 
  //   // history[count] = malloc(sizeof(char) * 81); 
  //   // scanf ("%[^\n]%*c", line);
  //   // strcpy(history[count], line); 
  //   // free(line); 
  //   // count++; 
  //   char* values= "!1"; 

  //   int a = values[0]; 
  //   int b = values[1]; 
  //   printf("%d\n", a);
  //   printf("%d\n", b-48);  
  //   count++; 

  // // }
  

  // // printHistory();
  // // arrayPlay(history); 

  // // printf("%s, %s, %s, %s\n", history[0], history[1], history[2], history[3]); 

  
  // // printf("%s %s %s \n", a, b,  c); 
   return 0;
}



double getAverage(int* arr, int size) {

   int  i, sum = 0;       
   double avg;          
 
   for (i = 0; i < size; ++i) {
      sum += arr[i];
   }
 
   avg = (double)sum / size;
   return avg;
}