// LEAVE THIS FILE IN THE DEFAULT PACKAGE
//  (i.e., DO NOT add 'package cs311.pa1;' or similar)

// DO NOT MODIFY THE EXISTING METHOD SIGNATURES
//  (you may, however, add member fields and additional methods)

// DO NOT INCLUDE LIBRARIES OUTSIDE OF THE JAVA STANDARD LIBRARY
//  (i.e., you may include java.util.ArrayList etc. here, but not junit, apache commons, google guava, etc.)

/**
* @author Anthony House, Ben Holmes
*/

public class Tuple
{
	// member fields and other member methods
	int key; 
	String value; 

	public Tuple(int keyP, String valueP)
	{
		// implementation
		this.key = keyP; 
		this.value = valueP; 
	}

	public int getKey()
	{
		// implementation
		return key; 
	}

	public String getValue()
	{
		// implementation
		return value; 
	}

	public boolean equals(Tuple t)
	{
		// implementation
		boolean result = false; 
		if(t.key == this.key && t.value.equals(this.value)) {
			result = true; 
		}
		return result; 
	}
}