/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

void getFileCreationTime(char *path) {
    struct stat attr;
    stat(path, &attr);
    printf("Last modified time: %s \n\n", ctime(&attr.st_mtime));
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}

#define MAX_LINE 255

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


char* getFile(char* file){
    char *source = NULL;
    FILE *fp = fopen(file, "r");
    if (fp != NULL) {
        if (fseek(fp, 0L, SEEK_END) == 0) {
            long bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }

            source = malloc(sizeof(char) * (bufsize + 1));

            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            size_t newLen = fread(source, sizeof(char), bufsize, fp);
            if ( ferror( fp ) != 0 ) {
                fputs("Error reading file", stderr);
            } else {
                source[newLen++] = '\0'; 
            }
        }
        fclose(fp);
    }
    return source; 
}


void setUpServer(int sockfd, int portno, struct sockaddr_in* serv_addr){

    serv_addr->sin_family = AF_INET;
    serv_addr->sin_addr.s_addr = INADDR_ANY;
    serv_addr->sin_port = htons(portno);
    if (bind(sockfd, serv_addr,
         sizeof(*serv_addr)) < 0) 
         error("ERROR on binding");
}

void createSocket(int* sockfd){
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) 
       error("ERROR opening socket");
}

char* getStatus(char* file){
    char* result; 
    char* okStatus = "\r\n\r\nHTTP/1.1 200 OK\r\n\r\n"; 
    char* notFound = "\r\n\r\nHTTP/1.1 404 Not Found\r\n\r\n"; 
    char* notModified = "\r\n\r\nHTTP/1.1 304 Not Modified\r\n\r\n"; 

    if( access( file, R_OK ) != -1 ) {
        // file exists and is readable
        result = okStatus; 
    } else {
        // file can't be read
        result = notFound; 
    }

    return result; 
}

char* getDirectory(char* splitDirectories){
    int count = 0; 
    char** tokens = splitLine(splitDirectories, &count, '/'); 
    char* result; 
    int position = 0; 
    if(tokens[position]) result = malloc(sizeof(char)*strlen(tokens[position])); 
    while(tokens[position]){
        strcat(result, tokens[position]); 
        position++; 
        if(tokens[position]){
            result = realloc(result, sizeof(char) * strlen(tokens[position]) + 1); 
            strcat(result, "/"); 
        } 
    }
    result[strlen(result)] = '\0'; 

    return result; 
}

char* getResponse(int *newsockfd, int* header, char* buffer){

    int count = 0; 
    char** tokens = splitLine(buffer, &count, ' '); 
    char* file = getDirectory(tokens[1]); 

    getFileCreationTime(file); 

    printf("FILE IS %s \n\n\n", file); 

    if( access( file, R_OK ) != -1 ) {
        char* status = getStatus(file); 
        char* response = malloc(sizeof(char*) * strlen(status));
        strcat(response, status); 

        char* body = getFile(file);
        response = realloc(response, sizeof(char)*strlen(body)+5); 
        strcat(response, body); 

        strcat(response, "\r\n\r\n"); 
        response[strlen(response)] = '\0'; 

        return response; 

    } else {
        char* status = getStatus(file); 
        char* response = malloc(sizeof(char*) * strlen(status));
        strcat(response, status); 

        return response; 
    }
}


int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    createSocket(&sockfd); 

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    setUpServer(sockfd, portno, &serv_addr); 
    listen(sockfd,5);


    while(1){
      
        int header;

        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
             error("ERROR on accept");

        pid_t pid = fork(); 
        if(pid == 0){
            bzero(buffer,256);
            header = read(newsockfd,buffer,255);
            if (header < 0) error("ERROR reading from socket");

            printf("Request is : %s\r\n\r\n",buffer);


            char* response = getResponse(&newsockfd, &header, buffer); 
            int writeMe = write(newsockfd, response, strlen(response)); 

            if(writeMe < 0) error("ERROR writing response to client"); 


            free(response); 
        }

        sleep(1); 
    
    }

     return 0; 
}