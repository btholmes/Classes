import java.util.ArrayList;

// LEAVE THIS FILE IN THE DEFAULT PACKAGE
//  (i.e., DO NOT add 'package cs311.pa1;' or similar)

// DO NOT MODIFY THE EXISTING METHOD SIGNATURES
//  (you may, however, add member fields and additional methods)

// DO NOT INCLUDE LIBRARIES OUTSIDE OF THE JAVA STANDARD LIBRARY
//  (i.e., you may include java.util.ArrayList etc. here, but not junit, apache commons, google guava, etc.)

/**
* @author Ben Holmes, Anthony House
*/

public class HashCodeSimilarity
{
	private String s1; 
	private String s2; 
	private int length; 
	private int prime = 3; 
	
	private HashTable S; 
	private ArrayList<Integer> s1DistinctHashes; 
	
	private HashTable T; 
	private ArrayList<Integer> s2DistinctHashes; 
	
	private ArrayList<Integer> distinctHashes; 

	public HashCodeSimilarity(String s1, String s2, int sLength)
	{
		this.s1 = s1; 
		this.s2 = s2; 
		this.length = sLength; 
		
		S = new HashTable(s1.length());
		s1DistinctHashes = new ArrayList<Integer>(); 
		T = new HashTable(s2.length()); 
		s2DistinctHashes = new ArrayList<Integer>(); 
		
		distinctHashes = new ArrayList<Integer>(); 
		
		hashItOut(s1, true); 
		hashItOut(s2, false); 
	}
	
	public HashTable getS() {
		return S; 
	}
	
	public HashTable getT() {
		return T; 
	}
	
	public void addToS1(String string, int hash) {

		Tuple tuple = new Tuple(hash, string);  
		
		if(S.search(tuple) == 0) s1DistinctHashes.add(hash); 
		
		S.add(tuple);
		
		if(!distinctHashes.contains(hash)) {
			distinctHashes.add(hash); 
		}
		
	}
	
	public void addToS2(String string, int hash) {
		Tuple tuple = new Tuple(hash, string); 
		
		if(T.search(tuple) == 0) s2DistinctHashes.add(hash); 
		
		T.add(tuple); 
		
		if(!distinctHashes.contains(hash)) {
			distinctHashes.add(hash); 
		}
	}
	
	public int convertToInt(char ch) {
		return ch - 96; 
	}
	
	public int computeHash(String string) {
		int result = 0; 
		
		for(int i = 0; i < length; i++) {
			result += convertToInt(string.charAt(i)) * Math.pow(prime, i); 
		}
		
		return result; 
	}
	
	/**
	 * Uses Roll-over hashing method
	 * @param mainString
	 * @param s1
	 */
	public void hashItOut(String mainString, boolean s1) {
		if(mainString.length() < length) {
			return; 
		}
		if(mainString.length() == length) {
			if(s1) addToS1(mainString, mainString.hashCode()); 
			else addToS2(mainString, mainString.hashCode()); 
		}
		
		String firstString = mainString.substring(0, length); 
		int hash = computeHash(firstString); 
		
		for(int i = 0; i <= mainString.length()-length-1; i++) {
//			String string = mainString.substring(i, i+length); 
//			if(s1) addToS1(string, string.hashCode()); 
//			else addToS2(string, string.hashCode()); 
			if(s1) addToS1(firstString, hash); 
			else addToS2(firstString, hash); 
			firstString = mainString.substring(i+1, i+1+length); 
			hash -= convertToInt(mainString.charAt(i)); 
			hash = hash/prime; 
			hash += mainString.charAt(i+length) * Math.pow(prime, length -1); 
		}
	}

	
	public float lengthOfS1()
	{
		float result = 0.0f; 
		if(s1.length() < length) return 0.0f; 
		
		for(Integer hash : s1DistinctHashes) {
			ArrayList<Tuple> list = S.search(hash); 
				result += Math.pow(list.size(), 2);  
		}
		
		return (float) Math.sqrt(result); 
	}

	public float lengthOfS2()
	{
		float result = 0.0f; 
		if(s2.length() < length) return 0.0f; 
		
		for(Integer hash : s2DistinctHashes) {
			ArrayList<Tuple> list = T.search(hash); 
				result += Math.pow(list.size(), 2);  
		}
		
		return (float) Math.sqrt(result); 
		
	}

	public float similarity()
	{
		float result = 0.0f; 
		long topSummation = 0; 
		
		for(Integer hash : distinctHashes) {
			ArrayList<Tuple> list1 = S.search(hash); 
			ArrayList<Tuple> list2 = T.search(hash); 
			topSummation += (list1.size() * list2.size()); 
		}
		
		float denominator = (this.lengthOfS1() * this.lengthOfS2()); 
		
		if(denominator == 0) return 0.0f; 

		result = topSummation/denominator; 
		return result; 
	}
}