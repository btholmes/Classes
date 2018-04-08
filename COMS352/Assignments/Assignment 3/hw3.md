Ben Holmes
COMS 352
Assignment 3

# Problem 3:21

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int n=0;
	int m=0;
	
	pid_t pid;

		while(m <= 0)
		{
			printf("Enter a number greater than 0.\n"); 
  			scanf("%d", &m);	
		}

		pid = fork();

		if (pid == 0)
		{
			printf("Child is working...\n");
			printf("%d\n",m);
			while (m!=1)
			{
				if (m%2 == 0)
				{
					m = m/2;
				}
				else if (m%2 == 1)
				{
					m = 3 * (m) + 1;
				}	
			
				printf("%d\n",m);
			}
		
			printf("Child process is done.\n");
		}
		else
		{
			wait();
			<!-- End of parent process -->
		}
	return 0; 
}


# 3.22 


#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	const char* name = "Shared_Memory";
	const int SIZE = 4096 * 30;
	void *ptr;
	int shared_mem;
	int n = 0; 

	pid_t pid;


	while(n <=0){
		printf("Enter a number greater than 0.\n"); 
		scanf("%d", &n);	
	}
	


	shared_mem = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shared_mem, SIZE);
	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shared_mem, 0);
	
	if (ptr < 0) perror("Error with mmap in parent process");
	
	pid = fork();
		
	if (pid == 0) {
	    shared_mem = shm_open(name, O_RDWR, 0666);
	    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shared_mem, 0);
	    if (ptr < 0) perror("Error with map in child process");

	    while (n != 1) {
	        sprintf(ptr, "%d", n);
	        ptr++;
	        if (n % 2 == 0) n /= 2;
	        else n = 3 * n + 1;
	    }
	    
	    sprintf(ptr, "%d", n);
	    ptr++;
	} 
	else {
	    wait();
	    printf("Shared Memory is: %s\n", (char*) ptr);

	    shm_unlink(name);
	}
return 0;
}






(CHANGE)
# 4.7 
	When a kernel thread suffers a page fault, another kernel thread can be switched in to use the interleaving time in a useful manner. A single-threaded 
	process, on the other hand, will not be capable of performing useful work when a page fault takes place. Therefore, in scenarios where a program might suffer from frequent page faults or has to wait for other system events, a multithreaded solution would perform better even on a single-processor system. 

(GOOD)
# 4.8 

	Heap Memory and Global variables are shared across threads in a multithreaded process, but each thread has its own set of register values, and stack. 

(CHANGE)
# 4.11

	Though it is not possible to have parallelism without concurrency, it is possible to have concurrency but not parallelism.

# 4.17 

	Child = 5, parent = 0
o The parent forks a child process which has its own memory space, then waits for the forked child process to complete. The child process creates a new thread with the same memory space as the child process, and changes the value of the global variable to 5. Then it outputs the value of that variable which is 5 and the child process finishes. In the parent process, the value of the global variable remains unchanged and its value is outputted, which is 0.

# 4.18

	a) When the number of kernel threads is less than the number of processors, then some of the processors would remain idle since the scheduler maps only kernel threads to processors and not user-level threads to processors. 

	b) When the number of kernel threads is exactly equal to the number of processors, then it is possible that all of the processors might be utilized simultaneously. However, when a kernel thread blocks inside the kernel (due to a page fault or while invoking system calls), the corresponding processor would remain idle. 

	c) When there are more kernel threads than processors, a blocked kernel thread could be swapped out in favor of another kernel thread that is ready to execute, thereby increasing the utilization of the multiprocessor system.


