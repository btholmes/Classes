import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

// LEAVE THIS FILE IN THE DEFAULT PACKAGE
//  (i.e., DO NOT add 'package cs311.pa1;' or similar)

// DO NOT MODIFY THE EXISTING METHOD SIGNATURES
//  (you may, however, add member fields and additional methods)

// DO NOT INCLUDE LIBRARIES OUTSIDE OF THE JAVA STANDARD LIBRARY
//  (i.e., you may include java.util.ArrayList etc. here, but not junit, apache commons, google guava, etc.)

/**
* @author Anthony House, Ben Holmes
*/

public class HashTable
{
	int size = 0; 
	HashFunction hashFunc; 
	int numOfElements; 
	ArrayList<LinkedList<Tuple>> hashTable; 

	/**
	 * There has to be a better way to instantiate the ArrayList of new LinkedLists than this for loop
	 * @param size
	 */
	public HashTable(int size)
	{
		hashFunc = new HashFunction(size); 
		this.size = hashFunc.getPrime(); 
		numOfElements = 0; 
		
		hashTable = new ArrayList<LinkedList<Tuple>>(Collections.nCopies(this.size, new LinkedList<Tuple>())); 
		for(int i = 0; i < this.size; i++) {
			hashTable.set(i, null);  
		}

	}
	
	/**
	 * Method to retrieve the actual table, just for testing, to print out the hash table
	 * @return
	 */
	public ArrayList<LinkedList<Tuple>> getTable(){
		return hashTable; 
	}
	
	public void printTable() {
		System.out.println();
		for(int i =0; i < hashTable.size(); i++) {
			System.out.print(i + "  : ");
			LinkedList<Tuple> list = hashTable.get(i); 
			if(list != null) {
				for(int j =0; j < list.size(); j++) {
					Tuple tuple = list.get(j); 
					System.out.print(tuple.key + " , " + tuple.value + " - ");
				}
			}
			System.out.println();
		}
		System.out.println();
	}

	/**
	 * 
	 * @return the maximum LinkedList size stored in the hashTable ArrayList. 
	 */
	public int maxLoad()
	{
		int max = 0; 
		for(int i =0; i < hashTable.size(); i++) {
			if(hashTable.get(i) != null) max = Math.max(max,  hashTable.get(i).size()); 
		}
		return max; 
	}

	/**
	 * 
	 * @return the average of total elements/non null cells
	 */
	public float averageLoad()
	{
		int nonNullCells = 0; 
		for(int i =0; i < hashTable.size(); i++) {
			if(hashTable.get(i) != null) {
				nonNullCells++; 
			}
		}
		if(nonNullCells == 0) return 0; 
		
		return numOfElements/nonNullCells; 
	}

	public int size()
	{
		return this.size; 
	}

	public int numElements()
	{
		return this.numOfElements; 
	}

	/**
	 * 
	 * @return ratio of the number of elements to the total size of the table
	 */
	public float loadFactor()
	{
		if(size == 0) return 0f; 
		
		return numOfElements/size; 
	}

	/**
	 * Add new tuple to hashTable, update maxLoad, and numOfElements
	 * @param t
	 */
	public void add(Tuple t)
	{
		int hash = hashFunc.hash(t.getKey()); 
		
		if(hashTable.get(hash) == null) { 
			hashTable.set(hash, new LinkedList<Tuple>());  
			hashTable.get(hash).addFirst(t); 
		}
		else hashTable.get(hash).addFirst(t); 

		numOfElements++; 		
	}

	/**
	 * 
	 * @param k int key 
	 * @return array list of all tuples with given key
	 */
	public ArrayList<Tuple> search(int k)
	{
		List<Tuple> result = new ArrayList<Tuple>(); 
		int hash = hashFunc.hash(k); 
		
		if(hashTable.get(hash) == null) return (ArrayList<Tuple>) result; 
		
		result.addAll(hashTable.get(hash)); 
		
		
//		Iterator<Tuple> it = hashTable.get(hash).iterator(); 
//		while(it.hasNext()) {
//			Tuple tuple = it.next(); 
//			if(tuple.key == k) {
//				result.add(tuple); 
//			}
//		}
		
		
		return (ArrayList<Tuple>) result; 
	}

	/**
	 * 
	 * @param t Tuple
	 * @return number of occurences of a given Tuple in the hashTable
	 */
	public int search(Tuple t)
	{
		int hash = hashFunc.hash(t.getKey()); 
		int result = 0; 
		
		if(hashTable.get(hash) == null) return 0; 
		
		Iterator<Tuple> it = hashTable.get(hash).iterator(); 
		while(it.hasNext()) {
			if(it.next().equals(t)) {
				result++; 
			}
		}
		return result; 
	}

	/**
	 * Removes first occurence of the given tuple inside of hashTable
	 * @param t
	 */
	public void remove(Tuple t)
	{	
		int hash = hashFunc.hash(t.getKey()); 
		if(hashTable.get(hash) == null) return; 
		
		for(int i =0; i < hashTable.get(hash).size(); i++) {
			if(hashTable.get(hash).get(i).equals(t)) {
				hashTable.get(hash).remove(i); 
				numOfElements--; 
				return; 
			}
		}
	}
}