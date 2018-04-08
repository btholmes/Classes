COMS 311 HW 2 
Ben Holmes 


Problem 2: 


2) 		Overview: 
			The algorithm starts at 1, and sets a pointer there called j. It then loops through an array of size n, from 1 to n-1. At each iteration, it compares index j with index (j-1). 
			If a[j-1] < a[j], it increments j by 1, but if a[j-1] > a[j], it swaps(a[j], a[j-1]), decrements j by 1, and then continues comparing each a[j] with a[j-1] until j == 0, or a[j-1] < a[j]. 
	

	a) 	Worst Case : Reverse Sorted runs in O(n^2)
			In this case, at the start of each iteration i from 1 to n-1, there will be i elements to the left, which are greater than the element at a[i]. So, there will be i additional executions 
			of the while loop while each element slowly makes its way back to the front of the array. Since each execution in the while loop only takes constant time, we have : 
			
			c*1 + c*2 + c*3 .... c(n-1) This sum is an arithmetic series that goes up to (n-1).
			Using the formula for the arithmetic series, we get: 
			c*(n-1+1)((n-1)/2) = cn^2/2 - cn/2



		Best Case : Sorted runs in O(n)
			In this scenario, the inner while loop will never be executed, because there will never be a time when a[j-1] < a[j]. Therefore the total time is just the constant time it takes to check the 
			statment, a[j-1] > a[j], which is the summation from 1 to (n-1) of c, which is c(n-1): 
			
			c(n-1) which is O(n). 




	b) 	Total time for SORTED selection of 3000 is : 20
		Total time for REVERSED selection of 3000 is : 20
		Total time for RANDOM selection of 3000 is : 7

		Total time for SORTED bubble of 3000 is : 19
		Total time for REVERSED bubble of 3000 is : 14
		Total time for RANDOM bubble of 3000 is : 2

		Total time for SORTED insertion of 3000 is : 16
		Total time for REVERSED insertion of 3000 is : 27
		Total time for RANDOM insertion of 3000 is : 5

		Total time for SORTED selection of 30000 is : 1268
		Total time for REVERSED selection of 30000 is : 1246
		Total time for RANDOM selection of 30000 is : 1271


		Total time for SORTED bubble of 30000 is : 214
		Total time for REVERSED bubble of 30000 is : 197
		Total time for RANDOM bubble of 30000 is : 198


		Total time for SORTED insertion of 30000 is : 168
		Total time for REVERSED insertion of 30000 is : 176
		Total time for RANDOM insertion of 30000 is : 161

		Total time for SORTED selection of 300000 is : 128856
		Total time for REVERSED selection of 300000 is : 129915
		Total time for RANDOM selection of 300000 is : 129130


		Total time for SORTED bubble of 300000 is : 20875
		Total time for REVERSED bubble of 300000 is : 20906
		Total time for RANDOM bubble of 300000 is : 20792


		Total time for SORTED insertion of 300000 is : 19859
		Total time for REVERSED insertion of 300000 is : 18763
		Total time for RANDOM insertion of 300000 is : 18794