#include <string.h>
#include <stdio.h>
#include <stdlib.h> 


void printTokens(char* token); 

int main () {
   char str[80] = "date";
   const char* s = ";";
   char* tokenArray[7];
   char* copy; 
   copy = malloc(sizeof(char)*80); 
   
   /* get the first token */
   tokenArray[0] = str; 
   strcpy(copy, str); 
   copy = strtok(copy, " "); 
   printf("%s\n", copy); 
   // strcpy(copy, tokenArray[0]); 

   // copy = strtok(copy, s); 
   // printTokens(copy);

   // free(copy); 
   // copy = malloc(sizeof(char)*80); 

   // strcpy(copy, tokenArray[0]); 
   // copy = strtok(copy, s); 
   // printTokens(copy); 

   free(copy); 

   return(0);
}

void printTokensReverse(char* token){

   
   while(token != NULL){

   }

}


void printTokens(char* token){

   while( token != NULL ) {
      printf( " %s\n", token );
    
      token = strtok(NULL, ";");
   }

}