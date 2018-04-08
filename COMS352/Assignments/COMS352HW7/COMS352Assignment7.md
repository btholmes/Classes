Ben Holmes
COMS 352 
Assignment 7 



8.13) 
	a. External fragmentation
		This happens when too much memory is dynamically allocated, and not all of it can be used. If too much fragmentation happens, it reduces the amount of memory
		available. Total memory exists to satisfy a request, but it is not contiguous. 

	b. Internal fragmentation
		This is when allocated memory blocks have restriction on allowed sizes, and thus causes space to be wasted, because allocated memory can be larger than requested
		memory. The size difference is memory internal to a partition. 

	c. Ability to share code across processes
		This is made possible by sharing pages. If two processes need the same code, they reference the same page. 



8.20) 
	a. 3085
	b. 42095
	c. 215201
	d. 650000

	SOLUTION : 
		Length of offset is 10bits. 

	a) 3085
		step1: 
		3085 is 0000 1100 0000 1101. In 32 bit addressing is 0000 0000 0000 0000 0000 1100 0000 1101.

		Step2: 
			Page number = 0000000000000000000011
			Offset = 0000001101
		
		page number=3 and offset = 13.

		b) 42095

			step1: 
			42095 is 1010 0100 0110 1111. In 32 bit addressing is 0000 0000 0000 0000 1010 0100 0110 1111.

			Step2: 

				Page number = 00000000000000001010 01
				Offset = 0001101111

			page number=41 and offset = 111.

		c) 215201

		step1: Convert the decimal number into binary.
		215201 is 11 0100 1000 1010 0001. In 32 bit addressing is 0000 0000 0000 0011 0100 1000 1010 0001.

		Step2:

			Page number = 0000000000000011010010
			Offset = 0010100001

		page number=210 and offset = 161.

		d) 650000

		step1: 
		650000 is 1001 1110 1011 0001 0000. In 32 bit addressing is 0000 0000 0000 1001 1110 1011 0001 0000.

		Step2: 

			Page number = 0000000000001001111010
			Offset = 1100010000

		page number=634 and offset = 784.


	8.28) 
		QUESTION : 
			(a) 0,430
			(b) 1,10
			(c) 2,500
			(d) 3,400
			(e) 4,112

		SOLUTION : 
			(a) 219 + 430 = 649
			(b) 2300 + 10 = 2310
			(c) illegal reference; traps to operating system
			(d) 1327 + 400 = 1727
			(e) illegal reference; traps to operating system


	8.29) 
		QUESTION : 
			"What is the purpose of paging the page tables?" 

		SOLUTION : 
			If page tables are extremely large, paging them allows to manage memory by swapping portions of page tables which aren't being used. 

	9.3) 
		SOLUTION : 
			• 9EF - 0EF
			• 111 - 211
			• 700 - D00
			• 0FF - EFF


	9.8) 
		QUESTION Consider Ref String 
			 1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3,6
		
		SOLUTION : 
			   FRAMES    1    2    3    4     5    6    7
       
         	    LRU      20   18   15   10    8    7    7
          
         	   FIFO      20   18   16   14    10   10   7
         
       		 Optimal     20   15   11    8     7    7   7   




