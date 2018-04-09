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

#define MAX_LINE 255

// Returns the last time specified file was modified in format such as
// Sun Apr 8 04:05:56 2018
time_t getFileCreationTime(char *path) {
    struct stat attr;
    if(stat(path, &attr)==0){
        return attr.st_mtime;
    }
    return 0; 
}

//prints an error message than exits, used if input isn't formatted correctly
void error(char *msg)
{
    perror(msg);
    exit(1);
}

//Mainly used to split the http request by spaces, and then filter through the tokens
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

//Reads the specified file into a char* called source, then returns it
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

    // printf("Body in getFile : %s\n", source); 
    return source; 
}

// Basic server setup
void setUpServer(int sockfd, int portno, struct sockaddr_in* serv_addr){

    serv_addr->sin_family = AF_INET;
    serv_addr->sin_addr.s_addr = INADDR_ANY;
    serv_addr->sin_port = htons(portno);
    if (bind(sockfd, serv_addr,
         sizeof(*serv_addr)) < 0) 
         error("ERROR on binding");
}

//Creates the basic socket
void createSocket(int* sockfd){
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) 
       error("ERROR opening socket");
}


// This function is not being used 
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

//splits the resource identifier from the http request by the '/' char, then reassembles it into a fileName which
//can be passed to getFile(char* fileName) in order to retrieve the contents. 
void getDirectory(char* splitDirectories, char* result){
    memset(result, 0, 1000);


    int count = 0; 
    char** tokens = splitLine(splitDirectories, &count, '/'); 
    int position = 0; 
    int length = 0; 

    while(tokens[position]){
        length += strlen(tokens[position]); 
        strcat(result, tokens[position]); 
        position++; 
        if(tokens[position]){
            strcat(result, "/"); 
            length+=1; 
        } 
    }
    result[length] = '\0'; 
    free(tokens); 

}

//Determines if the httpRequest includes the HEAD parameter, if it does, 
// it marks the hFlag signfiying that it was found. 
void getHFlag(char** tokens, int* hFlag){

    if(!strcmp(tokens[0], "HEAD")){
        *hFlag = 1; 
    }else{
        *hFlag = 0; 
    }

}

//Used for getting the corresponding numerical value of a given month. 
//Useful when constructing the time_t structure from a char * date such as
// Sun Apr 8 04:05:56 2018
int getMonthAsNum(char* month){
    int result = 0; 

    if(!strcmp(month, "Jan")) result = 0; 
    else if(!strcmp(month, "Feb")) result = 1; 
    else if(!strcmp(month, "Mar")) result = 2; 
    else if(!strcmp(month, "Apr")) result = 3; 
    else if(!strcmp(month, "May")) result = 4; 
    else if(!strcmp(month, "Jun")) result = 5; 
    else if(!strcmp(month, "Jul")) result = 6; 
    else if(!strcmp(month, "Aug")) result = 7; 
    else if(!strcmp(month, "Sep")) result = 8; 
    else if(!strcmp(month, "Oct")) result = 9; 
    else if(!strcmp(month, "Nov")) result = 10; 
    else if(!strcmp(month, "Dec")) result = 11; 

    return result; 
}

//Takes the timeArg from the http request's If-Modified_Since parameter, and decomposes it into a time_t
//so that it can be compared to another time_t. 
time_t makeTime(char* tArg){
    // Sun Apr 8 04:05:56 2018
    char day[4]; 
    char month[4]; 
    int dayNum, hour, min, sec, year; 

    time_t result = 0; 

    if (sscanf(tArg, "%s %s %2d %2d:%2d:%2d %4d", day, month, &dayNum, &hour, &min, &sec, &year) == 7) {
        month[3] = '\0'; 
        day[3] = '\0'; 

        int monthAsNum = getMonthAsNum(month); 

        struct tm breakdown = {0};
        breakdown.tm_year = year - 1900; /* years since 1900 */
        breakdown.tm_mon = monthAsNum;
        breakdown.tm_mday = dayNum;
        breakdown.tm_hour = hour-1;
        breakdown.tm_min = min;
        breakdown.tm_sec = sec; 
     
        if ((result = mktime(&breakdown)) == (time_t)-1) {
           fprintf(stderr, "Could not convert time input to time_t\n");
           return EXIT_FAILURE;
        }
    

    }else{
        fprintf(stderr, "Failed to parse date modified string supplied \n\n"); 
        exit(1); 

    }

    return result; 

}

// Checks for the If-Modified_Since parameter in the http request, marks a flag to simbolize 
// it was found, then constructs the tArg from that parameter, and passes it to makeTime(char* tArg)
// in order to convert the char* into a time_t
void getTFlag(char** tokens, int* tFlag, char* tArg, time_t* modifiedSince){
// HEAD /var/www/html/index.html HTTP/1.1
// Host: localhost
// If-Modified-Since: Sun Apr  8 04:35:24 2018

    memset(tArg, 0, 1000); 

    int position = 0; 
    while(tokens[position]){
        char* item = tokens[position]; 
        if(!strcmp(item, "If-Modified-Since:")){
            *tFlag = 1; 
            position++; 
            if(!tokens[position]){
                fprintf(stderr,"TimeArg, Required time arg not provided\n");
                exit(1);
            }else{
                strcpy(tArg, tokens[position]); 
                int argPosition = position+1; 
                while(argPosition < (position+5)){
                    strcat(tArg, " "); 
                    strcat(tArg, tokens[argPosition]); 
                    argPosition++; 
                }
                *modifiedSince = makeTime(tArg); 
            }
            break; 
        }
        position++; 
    }

}

//This is the main function which constructs the response to be sent to the client. 
char* getResponse(int *newsockfd, int* header, char* buffer, char* response){
    memset(response, 0, 10000); 
    char* status; 

    int count = 0; 
    char** tokens = splitLine(buffer, &count, ' '); 

    char file[1000]; 
    getDirectory(tokens[1], file); 

    int hFlag = 0; 
    getHFlag(tokens, &hFlag); 

    int tFlag = 0; 
    char tArg[1000]; 
    time_t modifiedSince; 
    getTFlag(tokens, &tFlag, tArg, &modifiedSince); 


    free(tokens); 

    if( access( file, R_OK ) != -1 ) {

        if(tFlag && !hFlag){
            time_t createdOn =  getFileCreationTime(file);
            // printf("Last modified time: %s \n", ctime(&createdOn));
            //  printf("Param time: %s \n\n", ctime(&modifiedSince));
            if(modifiedSince <= createdOn){
                status = "\r\n\r\nHTTP/1.1 200 OK\r\n\r\n"; 
                strcat(response, status); 
                // printf("Modified on %s \n, Last modified param %s\n", ctime(&createdOn), ctime(&modifiedSince)); 
                char* body = getFile(file);
                strcat(response, body); 
                strcat(response, "\r\n\r\n"); 

                // printf("Response in getResponse %s\n", response); 
                free(body); 
                return response; 
            }else{
                status = "\r\n\r\nHTTP/1.1 304 Not Modified\r\n\r\n"; 
                strcat(response, status); 
                return response; 

            }

        }else if(!tFlag && hFlag){
            status = "\r\n\r\nHTTP/1.1 200 OK\r\n\r\n";
            strcat(response, status); 
            
            return response; 

        }else if(tFlag && hFlag){
            time_t createdOn =  getFileCreationTime(file);
            if(modifiedSince <= createdOn){
                status = "\r\n\r\nHTTP/1.1 200 OK\r\n\r\n"; 
                strcat(response, status); 

                return response; 

            }else{
                status = "\r\n\r\nHTTP/1.1 304 Not Modified\r\n\r\n"; 
                strcat(response, status); 

                return response; 
            }

        }else{
            status = "\r\n\r\nHTTP/1.1 200 OK\r\n\r\n";
            strcat(response, status); 

            char* body = getFile(file);
            strcat(response, body); 
            strcat(response, "\r\n\r\n"); 

            free(body); 
            return response; 
        }


    } else {
        status = "\r\n\r\nHTTP/1.1 404 Not Found\r\n\r\n";  
        strcat(response, status); 
        return response; 
    }


    return "Somehow this was returned"; 
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

            printf("\n\nRequest is : \n%s\r\n\r\n",buffer);


            char response[10000]; 
            getResponse(&newsockfd, &header, buffer, response);  

            int writeMe = write(newsockfd, response, strlen(response)); 

            if(writeMe < 0) error("ERROR writing response to client"); 

        }

        sleep(1); 
    
    }

     return 0; 
}