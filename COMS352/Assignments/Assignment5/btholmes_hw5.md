Ben Holmes
COMS 352 
HW 5



5.10. 

If a user-level program disables interrupts, then it can disable the timer interrupt and prevent context switching from taking place. 


5.11: 

Interrupts do not work in multiprocessor systems because disabling interrupts stops other processes from executing on the disabled processor. This means other processes could be executing on other processors, and gain access into a program that is already being accessed by a process on the processor without interrupts, therefore mutually exclusive access to programs cannot be guaranteed. 



5.16. 

Each mutex lock would need a queue to store waiting processes. When a process releases the mutex lock, it removes and wakes the first process from the waiting queue. 


5.23. 


int guard = 0;
int semaphore value = 0;
	wait()
	{
		while (TestAndSet(&guard) == 1);
		if (semaphore value == 0) {
			add process to a queue; 
			set guard to 0;
		} else {
			semaphore value--;
			guard = 0;
		}
	}
	signal()
	{
		while (TestAndSet(&guard) == 1);
			if (semaphore value == 0 && there is a process on the wait queue)
				//wake up the first process in the queue
			else
				semaphore value++;
				guard = 0;
	}


5.35. 

monitor alarm{
	
	int time = 0; 

	void delay (int ticks){

		int alarms; 
		alarms = time+ticks; 

		while(time < alarms){

			delay.wait(alarms); 
			delay.signal; 
		}

	}


	void tick(){
		time += 1; 
		delay.signal; 
	}

}



6. 
	Solution is attached in the threads.c code. 








