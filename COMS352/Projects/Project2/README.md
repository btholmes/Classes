# COMS 352 Server/Client in C
 * Ben Holmes
 * Spring 2018
 * Project 2

#### Note: httpClient.c and httpServer.c must be run in separate terminal windows

# Basic Instructions

 1. make server
 2. make client
 3. open a new terminal and navigate to project
 4. make run_server

 5. In terminal that's not running the server, execute make examples

	 		make run_client_1
 			make run_client_2
 			make run_client_3
 			make run_client_4
 			make run_client_5

 			make run_local_1
 			make run_local_2
 			make run_local_3
 			make run_local_4
 			make run_local_5


# Detailed Instructions (Below)

## httpClient.c

### This project requires that the url including port, and flags be provided as an argument to the httpClient

#### Below are a list of make examples to run client on a remote server

 * make run_client_1:

		./client https://www.w3.org/TR/html4/index/list.html -h -d 1:2:15 

 * make run_client_2:

		./client www.w3.org:80/TR/html4/index/list.html -h

 * make run_client_3: 

		./client www.w3.org:80/TR/html4/index/list.html -d 1:2:15

 * make run_client_4: 

		./client https://www.w3.org:80/TR/html4/index/list.html -h -d 1:2:15

 * make run_client_5:

		./client https://www.w3.org:80/TR/html4/index/list.html


##### All responses are saved to response.txt
 * All most recent requests are saved to response.txt file
 * New requests overwrite old requests in file, they are not appended. 


## httpServer.c

 1. Open a new terminal window and navigate to folder with httpServer.c
 2. make server
 3. make run_server

#### Below are a list of make examples to run on the local server 
 
 1. Go back to terminal window that isn't running the server

 * make run_local_1: 
 
			./client localhost:8765/var/www/html/index.html
 
 * make run_local_2:

			./client localhost:8765/var/www/html/index.html -h

 * make run_local_3:

			./client localhost:8765/var/www/html/index.html -h -d 0:0:0

 * make run_local_4:

			./client localhost:8765/var/www/html/index.html -d 20:0:0

 * make run_local_5:

			./client localhost:8765/var/www/html/random.html 