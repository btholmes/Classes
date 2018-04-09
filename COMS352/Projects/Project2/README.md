# COMS 352 Server/Client in C
 * Ben Holmes
 * Spring 2018
 * Project 2

## httpClient.c

### This project requires that the url including port, and flags be provided as an argument to the httpClient

#### Below are a list of make examples to run client on a remote server

 * run_client_1:
	
		./client https://www.w3.org/TR/html4/index/list.html -h -d 1:2:15 

 * run_client_2:
	
		./client www.w3.org:80/TR/html4/index/list.html -h

 * run_client_3: 
		
		./client www.w3.org:80/TR/html4/index/list.html -d 1:2:15

 * run_client_4: 
	
		./client https://www.w3.org:80/TR/html4/index/list.html -h -d 1:2:15


##### All responses are saved to response.txt
 * All most recent requests are saved to response.txt file
 * New requests overwrite old requests in file, they are not appended. 


## httpServer.c

### Port number must be supplied as argument as in step 3 below

 1. Open a new terminal window and navigate to folder with httpServer.c
 2. gcc -o httpServer httpServer.c -o -w
 3. ./httpServer portNo
 		./httpServer 8765

#### Below are a list of make examples to run on the local server (First make sure the local server is running)

 * make run_local_1
 			
 			./client localhost:8765/var/www/html/index.html

 * make run_local_2
	
			./client localhost:8765/var/www/html/index.html -h

 * make run_local_3
	
			./client localhost:8765/var/www/html/index.html -h -d 0:0:0

 * make run_local_4
	
			./client localhost:8765/var/www/html/index.html -d 20:0:0

 * make run_local_5
	
			./client localhost:8765/var/www/html/not_here.html 