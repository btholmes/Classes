CC=gcc
CFLAGS=-w.

client: httpClient.c
	$(CC) -w httpClient.c -o client 

server: httpServer.c
	$(CC) -w httpServer.c -o server

run_server:
	./server 8765

run_client_1:
	./client https://www.w3.org/TR/html4/index/list.html -h -d 1:2:15 

run_client_2:
	./client www.w3.org:80/TR/html4/index/list.html -h

run_client_3: 
	./client www.w3.org:80/TR/html4/index/list.html -d 1:2:15

run_client_4: 
	./client https://www.w3.org:80/TR/html4/index/list.html -h -d 1:2:15




run_local_1: 
	./client localhost:8765/var/www/html/index.html

run_local_2:
	./client localhost:8765/var/www/html/index.html -h

run_local_3:
	./client localhost:8765/var/www/html/index.html -h -d 0:0:0

run_local_4: 
	./client localhost:8765/var/www/html/index.html -d 20:0:0

run_local_5:
	./client localhost:8765/var/www/html/not_here.html 


clean: 
	rm -f client server