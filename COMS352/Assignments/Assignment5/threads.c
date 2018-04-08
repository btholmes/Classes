#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
/* External References */
extern void* world( void* x );
extern void* hello( void* x );
extern void* exclamation(void* x);

int main( int argc, char *argv[] ) {
	pthread_t hello_thread;
	pthread_t world_thread;
	pthread_t exclamation_thread;
	int x = 0; 

	if(pthread_create(&hello_thread, NULL, hello, &x)) {

		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	/* wait for the hello thread to finish */
	if(pthread_join(hello_thread, NULL)) {

		fprintf(stderr, "Error joining thread\n");
		return 2;
	}


	if(pthread_create(&world_thread, NULL, world, &x)){
			fprintf(stderr, "Error creating thread\n");
			return 1;
	}

		/* wait for the world thread to finish */
	if(pthread_join(world_thread, NULL)) {

		fprintf(stderr, "Error joining thread\n");
		return 2;
	}

	if(pthread_create(&exclamation_thread, NULL, exclamation, &x)){
			fprintf(stderr, "Error creating thread\n");
			return 1;
	}

	//pthread_yield doesn't seem to work with C99, I read sched_yield is the same things though.. 
	sched_yield(); 
	printf( "\n" );
	return( 0 );
}
/* world - print the "world" part. */
void* world( void* x ) {

	printf( "world");
	pthread_exit(NULL); 
}
/* hello - print the "hello" part. */
void* hello( void* x ) {

	printf( "hello ");
	pthread_exit(NULL); 
}
/* exclamation – print “!”.*/
void* exclamation(void* x){

	printf("!");
	pthread_exit(NULL); 
}