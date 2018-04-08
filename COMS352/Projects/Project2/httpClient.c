//FIXME: compile this with -lssl and -lcrypto
/****************************************************/
/*            INCLUDES          */
/****************************************************/ 

#define __USE_POSIX
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "sha1.h"
#include <unistd.h>
#include <math.h>
#include <netdb.h> //gethostbyname

#include <sys/types.h>
#include <time.h> 



#include <limits.h>

#define PORT 80
#define MAX_LINE 81
#define HOST_NAME_MAX 64

int hFlag = 0; 
int tFlag = 0; 
char* tArg; 


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

int getHFlag(char** args, int argc){
  int result = 0; 

  if(args[2]){
      if(!strcmp(args[2], "-h")){
        result = 1; 
      }
  }
  if(args[3]){
    if(!strcmp(args[3], "-h")){
        result = 1; 
      }
  }
   if(args[4]){
    if(!strcmp(args[4], "-h")){
        result = 1; 
      }
  }

  return result; 
}

char* getTArg(char** args, int* tFlag, int argc){
  char* result; 
  if(args[2]){
     if(!strcmp(args[2], "-d")){
        *tFlag = 1; 
        if(args[3])
          result = args[3]; 
      }
  }

   if(args[3]){
     if(!strcmp(args[3], "-d")){
        *tFlag = 1; 
        if(args[4])
          result = args[4]; 
      }
  }
  return result; 
}

int getPortNumber(char** portNumber){
  int result; 
    if(portNumber[1]){
      result = atoi(portNumber[1]); 
    }else{
      result = PORT; 
    }

    return result; 
}

char* ifModifiedSince(){
    int count = 0; 
    char** args = splitLine(tArg, &count, ':'); 
    int day = atoi(args[0]); 
    int hour = atoi(args[1]); 
    int min = atoi(args[2]); 

    char* s_time = malloc(sizeof(char)*30);  //value to be used in “If-Modified-Since” header
    time_t n_time;
    n_time=time(0);
    n_time=n_time-(day*24*3600+hour*3600+min*60);
    strcpy(s_time, ctime(&n_time)); 

    printf("Time is %s\n", s_time); 
  return s_time; 
}

char* getHeader(char* hostName, char* resource){
  char* type; 

  if(hFlag){
    type = "HEAD /"; 
  }else{
    type = "GET /"; 
  }

  char* protocol = " HTTP/1.1\r\n"; 
  char* host = "Host: "; 
  char* end = "\r\n\r\n"; 
  
  char* timeStuff = ""; 
  char* timeArg = ""; 
  if(tFlag){
    timeStuff = "If-Modified-Since: \r\n\r\n"; 
    timeArg = ifModifiedSince(); 
  }

  char *header = malloc(sizeof(char) * strlen(type) + sizeof(char)*strlen(protocol)+sizeof(char)*strlen(host)+
      sizeof(char)*strlen(end)+sizeof(char)*strlen(hostName)+sizeof(char)*strlen(resource) 
      + sizeof(char)*strlen(timeStuff) + sizeof(char)*strlen(timeArg)+ 1); 

  if(hFlag && !tFlag){
    sprintf(header, "HEAD /%s HTTP/1.1\r\nHost: %s\r\n\r\n", resource, hostName);
  }else if(hFlag && tFlag){
    sprintf(header, "HEAD /%s HTTP/1.1\r\nHost: %s\r\nIf-Modified-Since: %s\r\n\r\n", resource, hostName, timeArg);
  }else if (tFlag && !hFlag){
    sprintf(header, "GET /%s HTTP/1.1\r\nHost: %s\r\nIf-Modified-Since: %s\r\n\r\n", resource, hostName, timeArg);
  }else{
    sprintf(header, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", resource, hostName);
  }
  // char* header = "GET /index.html HTTP/1.1\r\nHost: www.cs.iastate.edu\r\nIf-Modified-Since: Mon, Mar 27 2018 16:45:15 GMT\r\n\r\n"; 
  // char* header = "GET /index.html HTTP/1.1\r\nHost: www.cs.iastate.edu\r\nIf-Modified-Since: Sun Mar 25 10:16:20 2018\r\n\r\n"; 

  // printf("\n\n"); 
  // printf("Header is %s\n", header);   

  return header; 
}

void writeToFile(char buf[]){
  FILE *f = fopen("response.txt", "w");
  if (f == NULL)
  {
      printf("Error opening file!\n");
      exit(1);
  }

  fprintf(f, "%s\n\n\n", buf);
}

void connectToServer(char* hostName, int port, struct sockaddr_in server_addr, int* sock, char* resource){
  // server.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buf[2056]; 
    int byte_count; 
    struct hostent *server; 
    server = gethostbyname(hostName); 

    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( port );

    //Connect to remote server
    if (connect(*sock , (struct sockaddr *)&server_addr , sizeof(server_addr)) < 0)
    {
        perror("connect failed. Error");
        return; 
    }

    char *header = getHeader(hostName, resource); 
    // printf("Header is %s\n" , header); 
    send(*sock,header,strlen(header),0);

    byte_count = recv(*sock,buf,sizeof(buf)-1,0); // <-- -1 to leave room for a null terminator
    buf[byte_count] = 0; // <-- add the null terminator
    printf("%s",buf);

    writeToFile(buf); 

    // free(header); 
    // puts("Connected\n");
}

void createSocket(int *sock){
//Create socket
    *sock = socket(AF_INET , SOCK_STREAM , 0);
    if (*sock == -1)
    {
        puts("Could not create socket");
    }
    // puts("enter URL to connect to.. http://www.cs.iastate.edu");
}

int startsWith(const char *a, const char *b)
{

   if(strncmp(a, b, strlen(b)) == 0) return 1;
    return 0;
}

char* getHost(char* urlCopy, char** domainPortResource){
  char* host; 
   if(startsWith(urlCopy, "http://") || startsWith(urlCopy, "https://")){
    host = malloc(sizeof(char) * strlen(domainPortResource[1] + 1)); 
    strcpy(host, domainPortResource[1]); 
  }else{
    host = malloc(sizeof(char) * strlen(domainPortResource[0] + 1)); 
    strcpy(host, domainPortResource[0]); 
  }

  return host; 
}

int main(int argc, char *argv[]){
  if (argc < 2) {
       fprintf(stderr,"usage %s url -h(optional) -d(optional) dateArgument\n", argv[0]);
       exit(0);
  }

  hFlag = getHFlag(argv, argc); 
  tArg =  getTArg(argv, &tFlag, argc); 

  // printf("H flag %d , Tflag %d \n", hFlag, tFlag); 

  int sock;
  int count; 
  struct sockaddr_in server;
  createSocket(&sock); 

  char* urlCopy = malloc(sizeof(char) * strlen(argv[1])+1); 
  strcpy(urlCopy, argv[1]); 

  char** domainPortResource = splitLine(argv[1], &count, '/'); 

  char* host = getHost(urlCopy, domainPortResource); 


  char* hostCopy = malloc(sizeof(char) * strlen(host) + 1); 
  strcpy(hostCopy, host); 

  char** portInfo = splitLine(hostCopy, &count, ':'); 
  int port = getPortNumber(portInfo); 

  char* resource = "index.shtml"; 
  connectToServer(host, port, server, &sock, resource); 


  // while(1){
  //   char* line; 
  //   char **args;
  //   int count = 0; 

    // int hFlag = 0; 
    // int tFlag = 0; 
    // char* tArg; 

  //   char** domainPortResource; 
  //   char** domainPortResourceCopy; 

  //   char** portInfo; 

  //   fflush(stdout); 

  //   line = getLine(); 

  //   args = splitLine(line, &count, ' '); 

    // hFlag = getHFlag(args); 
    // tArg =  getTArg(args, &tFlag); 

  //   char* args0Copy = malloc(sizeof(char) * strlen(args[0])+1); 
  //   strcpy(args0Copy, args[0]); 
  //   // domainPortResourceCopy = splitLine(args0Copy, &count, '/'); 

  //   domainPortResource = splitLine(args[0], &count, '/'); 
    
  //   char* host; 
  //   // if(startsWith(args0Copy, "http://") || startsWith(args0Copy, "https://")){
  //   //   host = malloc(sizeof(char) * strlen(domainPortResource[1] + 1)); 
  //   //   strcpy(host, domainPortResource[1]); 
  //   // }else{
  //     host = malloc(sizeof(char) * strlen(domainPortResource[0] + 1)); 
  //     strcpy(host, domainPortResource[1]); 
  //   // }
   
  //   char* hostCopy = malloc(sizeof(char) * strlen(host) + 1); 
  //   strcpy(hostCopy, host); 

  //   portInfo = splitLine(hostCopy, &count, ':'); 
  //   int port = getPortNumber(portInfo); 
 

  //   // char* resource = getResource(domainPortResourceCopy); 
  //   char* resource = "index.html"; 
  //   connectToServer(host, port, server, &sock, resource); 


  //   printf("%s\n", host); 
  //   printf("%d\n", port); 
  //   // printf("%s\n", resource); 


  //   free(line);
  //   free(args);
  //   free(args0Copy); 
  //   // free(domainPortResourceCopy); 
  //   free(domainPortResource); 
  //   free(host); 
  //   free(hostCopy); 
  //   free(portInfo); 
  //   // free(resource); 
 
    
  
  // } 


  return 0;
}