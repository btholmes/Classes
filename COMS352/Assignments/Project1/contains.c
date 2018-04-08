#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* getEverythingBeforeTheSemiColon(char* line, char* value); 

int main () {
   char* str = "http://www.tutorialspoint.com";
   const char ch = '.';
   char *ret;

   printf("Last char is %c", str[strlen(str)]); 
   ret = strchr(str, ch);


	char* result = getEverythingBeforeTheSemiColon(str, ret); 

   printf("\n Result is : %s \n", result); 

   // printf("Before string is %s \n", str[0]ret);
   
   return(0);
}

char* getEverythingBeforeTheSemiColon(char* line, char* value){

	char* result = (char*)malloc(sizeof(char)*strlen(line)); 
	int i = 0; 
   while(strcmp(line, value) != 0){
   	 result[i] = *line++; 
   	 i++; 
   }
   printf("\n"); 

   return result; 
}