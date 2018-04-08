#include <stdio.h>
#include <unistd.h>

void problem1(void);
void problem2(void); 

int main( ){
	// printf("%s", "Problem 1 is : \n" ); 
	// problem1(); 
	// printf("%s", "Problem 2 is \n"); 
	// problem2(); 
	int i;
	pid_t pid; 
 	for(i=0;i<2;i++) {
 		pid = fork();
 	}
 	printf("%d,%d   And pid is : %d\n", getppid(), getpid(), pid);

}

void problem1(){
 int i;
 for(i=0;i<2;i++) {
	 fork();
	 printf("%d,%d\n", getppid(), getpid());
 }

}

void problem2(){
	int i;
 	for(i=0;i<2;i++) fork();
 	printf("%d,%d\n", getppid(), getpid());

}