import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;


public class linked {
	public static LinkedList<Tuple> list; 
	public static ArrayList<LinkedList<Tuple>> hashTable; 
	
	public static ArrayList<Integer> s1DistinctHashes = new ArrayList<Integer>(); 
	public static ArrayList<Integer> s2DistinctHashes = new ArrayList<Integer>(); 
	
	public static ArrayList<Integer> s1Count = new ArrayList<Integer>(); 
	public static ArrayList<Integer> s2Count = new ArrayList<Integer>(); 

	
	public static void main(String[] args) {
		list = new LinkedList<>(); 
		Tuple tuple = new Tuple(1, "what"); 
		Tuple copy = new Tuple(1, "what"); 
		list.add(tuple); 

		
		addToS1(100);  
		addToS1(100); 
		addToS1(300); 
		addToS1(200); 
		addToS1(0); 
		
		System.out.println(getS1Count(0));
//		int val = distinctHashes.indexOf(200); 
//		System.out.println(val);
//		Arrays.fill(array, 0);
		
	}
	
	public static int getS1Count(int val) {
		if(!s1DistinctHashes.contains(val)) return 0; 
		
		int index = s1DistinctHashes.indexOf(val); 		
		return s1Count.get(index); 
	}
	
	public static void addToS1(int val) {
		if(!s1DistinctHashes.contains(val)) {
			s1DistinctHashes.add(val); 
			s1Count.add(1); 
		}else {
			int index = s1DistinctHashes.indexOf(val); 
			s1Count.set(index, s1Count.get(index)+1); 
		}	
	}
	
	public void printTable() {
		for(int i =0; i < hashTable.size(); i++) {
			System.out.println(hashTable.get(i));
		}
	}
	
	public static void remove(Tuple t) {
		int hash = 4; 
		for(int i =0; i < hashTable.get(hash).size(); i++) {
			if(hashTable.get(hash).get(i).equals(t)) {
				hashTable.get(hash).remove(i); 
				return; 
			}
		}
		
	}
}
