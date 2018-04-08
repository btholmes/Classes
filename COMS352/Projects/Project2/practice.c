#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>


#define MAX_LINE 80

char** splitLine(char *line, int *count, char splitBy){

   char delimiter[6] = {' ','\t','\r','\n','\a','\0'}; 
   delimiter[0] = splitBy; 

   char** tokens = malloc(sizeof(char*)*MAX_LINE); 
   
   int position = 0; 

   if(!tokens){
    fprintf(stderr, "Error allocatings space for tokens in split line"); 
    exit(EXIT_FAILURE); 
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
    // printf("%d\n", c); 
    if (c == EOF || c == '\n' || c == 10) {
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

char* getResource(char** domainPortResource){
    int position = 2;   
    char* result; 
    char* resource = domainPortResource[position]; 
    result = malloc(sizeof(char)*strlen(resource) + 1); 
    strcpy(result, resource); 
    position++; 
      while(domainPortResource[position]){
        result = realloc(result, sizeof(char)); 
        strcat(result, "/"); 
        resource = domainPortResource[position]; 
        result = (char*)realloc(result, sizeof(char)*strlen(resource)+1); 
        strcat(result, resource); 
        position++; 
      }
    return result; 
}

char* getHeader(char* hostName, char* resource){
  char* type = "GET /"; 
  char* protocol = " HTTP/1.1\r\n"; 
  char* host = "Host: "; 
  char* end = "\r\n\r\n"; 
  
  char *header = malloc(sizeof(char) * strlen(type) + sizeof(char)*strlen(protocol)+sizeof(char)*strlen(host)+
      sizeof(char)*strlen(end)+sizeof(char)*strlen(hostName)+sizeof(char)*strlen(resource) + 1); 

  sprintf(header, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", resource, hostName);
  // char* header = "GET /expertise/60 HTTP/1.1\r\nHost: www.cs.iastate.edu\r\n\r\n";

  return header; 
}


int main(){
	
  char* header = getHeader("www.cs.iastate.edu", "index.html"); 
  printf("%s\n", header );

	// char* line; 
 //    char **args;
 //    int count; 

 //    while(1){

 //    	line = getLine(); 

	//     args = splitLine(line, &count, ' '); 


	//  	char** domainPortResource = splitLine(args[0], &count, '/'); 


	//  	char* resource = getResource(domainPortResource); 

	//     // printf("%s\n", host); 
	//     // printf("%d\n", port); 
	//     printf("%s\n", resource); 


	//     free(line);
	//     free(args);
	//     free(resource); 
	//     free(domainPortResource); 
 //    }


}