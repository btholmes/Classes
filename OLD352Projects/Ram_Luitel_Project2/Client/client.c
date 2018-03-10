#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include "sha1.h"
char *data[3];
char hashed[1000];
 char signature[1000];
int binaryMap[8] = {6,7,5,4,3,2,0,1};
long decimalToBinary(long n) {
 int remainder;
 long binary = 0, i = 1;

    while(n != 0) {
        remainder = n%2;
        n = n/2;
        binary= binary + (remainder*i);
        i = i*10;
    }
    return binary;
}
void inplace_reverse(char * str)
{
  if (str)
  {
    char * end = str + strlen(str) - 1;

    // swap the values in the two given variables
    // XXX: fails when a and b refer to same memory location
#   define XOR_SWAP(a,b) do\
    {\
      a ^= b;\
      b ^= a;\
      a ^= b;\
    } while (0)

    // walk inwards from both ends of the string,
    // swapping until we get to the middle
    while (str < end)
    {
      XOR_SWAP(*str, *end);
      str++;
      end--;
    }
#   undef XOR_SWAP
  }
}


//binaryToDecimal will return the decimal number for the corresponding binary form. for eg. decimal [5] for binary [101].
int binaryToDecimal(long n)
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}


void stringToAscii(char *s){
  char *ptr = s;
  char finalRes[256]="";

  //Go through each character in the string and append the correpsonding ASCII representaion to the final result
  while(*ptr){
    int asciiDecimal = (int)*ptr;
    int res = decimalToBinary(asciiDecimal);

    char snum[5];
    sprintf(snum, "%d",res);
    strcat(finalRes,snum);

    ptr++;
  }

  //print the complete ASCII form
// printf("\nstringToAscii:: %s",finalRes);
  strcpy(signature,finalRes);
  return;
}
void stringToReverseAscii(char *s){
  char *ptr = s;
  char finalRes[256]="";

  //Visit each character in the string and append its ASCII representation to a temp result
  while(*ptr){
    int asciiDecimal = (int)*ptr;
    int res = decimalToBinary(asciiDecimal);

    char snum[5];
    sprintf(snum, "%d",res);
    strcat(finalRes,snum);

    ptr++;
  }

  //Reverse the temp result to get the final answer
  inplace_reverse(finalRes);

  //printf("\nstringToReverseAscii:: %s",finalRes);
  strcpy(signature,finalRes);
  return;
}


//stringToEncodedAscii prints the encoded representaion of the ASCII representaion of a given string(Encoding is based on encoding array at the top of this file)
void stringToEncodedAscii(char *s){
  char *ptr = s;
  char finalRes[256]="";

  //First, get the ASCII representation of the given string
  while(*ptr){
    int asciiDecimal = (int)*ptr;
    int res = decimalToBinary(asciiDecimal);

    char snum[5];
    sprintf(snum, "%d",res);
    strcat(finalRes,snum);

    ptr++;
  }

  //Since, we are encoding 3 bits at a time of the ASCII representaion, we need to pad extra 0s to the ASCII string to make
  //its length a multiple of 3.
  int lenOfFinalRes = strlen(finalRes);
  if((lenOfFinalRes % 3) != 0){
    while((lenOfFinalRes % 3) != 0){
      strcat(finalRes,"0");
      lenOfFinalRes++;
    }
  }

  //printf("\nString length : %d\n",lenOfFinalRes);


  char *newPtr = finalRes;
  int j,k;
  char finalResEncoded[256] = "";

  //We now take 3 bits at a time, find the encoding from encoding table at top and append the encoding to a new result string.
  for(j=0;j<(lenOfFinalRes/3);j++){
     char blk[10]= "";
     for(k=0;k<3;k++){
       //printf("\n next : %c\n",*newPtr);
       char ch[2];
       sprintf(ch, "%c",*newPtr);
       strcat(blk,ch);
       newPtr++;
     }

    long ret;
    char *ptr1;
    ret = strtol(blk, &ptr1, 10);
    int retI = binaryToDecimal(ret);
    int encodedRet = binaryMap[retI];
    int res = decimalToBinary(encodedRet);

    char snum[5];
    sprintf(snum, "%d",res);

	//The following is to make sure that, we always have a 3 bit binary representaion. For eg. decimal 3 is representated as 011 and not 11
    char paddedStr[5] = "";
    sprintf(paddedStr, "%s","");
    int snumLen = strlen(snum);
    if((snumLen%3) != 0){
      int z;
      for(z=0; z < (3-(snumLen%3)); z++){
        strcat(paddedStr,"0");
      }
    }

    strcat(paddedStr,snum);

   // printf("\nnormal : %ld ; retI : %d; encodedRet : %d; encoded : %s",ret,retI,encodedRet ,paddedStr);
    strcat(finalResEncoded,paddedStr);
  }

 // printf("\nstringToEncodedAscii :: %s",finalResEncoded);
  strcpy(signature,finalResEncoded);

}


int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        puts("Could not create socket");
    }
    puts("Socket created");


    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //keep communicating with server
    while(1)
    {
        puts("\nType message: \t");
        fflush(stdin);
        memset(message,0,strlen(message)+strlen(signature));
        fgets(message,1000,stdin);
        SHA1(hashed,message,strlen(message));
        strcpy(signature,hashed);
        stringToAscii(signature);
        stringToReverseAscii(signature);
        stringToEncodedAscii(signature);

        int i,j;

        if(send(sock,message,1000, 0) < 0)
        {
            puts("Send failed");
            return 1;
        }

        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

        if(strcmp(server_reply,signature)==0)
        	puts("true");
        else puts("false");

    }

    close(sock);
    return 0;
}
