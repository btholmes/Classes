# COMS 352 Server/Client in C
 * Ben Holmes
 * Spring 2018
 * Project 2

## httpClient.c

### This project requires that the url including port, and flags be provided as an argument to the httpClient

 * such as: 

	./httpClient https://www.w3.org/TR/html4/index/list.html -h -d 1:2:15 
	./httpClient www.w3.org:80/TR/html4/index/list.html -h
	./httpClient www.w3.org:80/TR/html4/index/list.html -d 1:2:15
	./httpClient https://www.w3.org:80/TR/html4/index/list.html -h -d 1:2:15


 * if you wish to connect to localhost, start server first in a new terminal window, then use use 

	./httpClient localhost:port/resource.ext


### Saved to response.txt
 * All most recent requests are saved to response.txt file
 * New requests overwrite old requests in file, they are not appended. 



## httpServer.c

### Port number must be supplied as argument as in step 3 below

 1. Open a new terminal window and navigate to folder with httpServer.c
 2. gcc -o httpServer httpServer.c -o -w
 3. ./httpServer portNo
 		./httpServer 8765