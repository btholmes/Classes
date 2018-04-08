import java.util.ArrayList;
import java.util.LinkedList;

public class main {

	public static HashTable hashTable; 
	
	public static void main(String[] args) {
		String s1 = "aroseisaroseisarose";
		String s2 = "aroseisaflowerwhichisarose"; 
		
		HashStringSimilarity stringSimilarity = new HashStringSimilarity(s1, s2, 4);
		
		System.out.println(stringSimilarity.similarity());
		
		BruteForceSimilarity brute = new BruteForceSimilarity(s1, s2, 4); 
		ArrayList<Integer> empty = new ArrayList<Integer>(); 
		System.out.println(empty.size());
		
		HashCodeSimilarity hashCodeSimilarity = new HashCodeSimilarity(s1, s2, 4); 
		System.out.println(hashCodeSimilarity.similarity());
		hashCodeSimilarity.getS().printTable();
		hashCodeSimilarity.getT().printTable(); 
//		

		
//		System.out.println('a'-96);
//		LinkedList<Tuple> list = new LinkedList<Tuple>(); 
//		list.add(new Tuple(1, "what")); 
		
	
		
//		hashTable = new HashTable(10); 
//		
//		for(int i = 0; i < 10; i++) {
//			Tuple tuple = new Tuple(i, "what"); 
//			Tuple tuple2 = new Tuple(0, "what"); 
//			hashTable.add(tuple);
//			hashTable.add(tuple2);
//		}
//		hashTable.remove(new Tuple(0, "what"));
//		hashTable.remove(new Tuple(0, "what"));
//		hashTable.remove(new Tuple(0, "what"));
//		hashTable.remove(new Tuple(0, "what"));
//		
//		printHashTable(); 
//		System.out.println();
//		System.out.println(hashTable.loadFactor());
//		System.out.println(hashTable.maxLoad());
//		System.out.println(hashTable.averageLoad());
//		System.out.println(hashTable.search(1).toString());
		
		
//		System.out.println(hashTable.numElements());
//		System.out.println("abc".hashCode());
//		System.out.println("abc".hashCode());
//		System.out.println(Math.sqrt(0));
		
	}
	public static void printHashTable() {
		ArrayList<LinkedList<Tuple>> table = hashTable.getTable(); 
		for(int i = 0; i < table.size(); i++) {
			System.out.print(i + "   ");
			LinkedList<Tuple> list = table.get(i); 
			if(list != null) {
				for(int j =0; j < list.size(); j++) {
					Tuple tuple = list.get(j); 
					System.out.print(tuple.getKey() + ", " + tuple.getValue() + " : ");
				
				}
			}else System.out.print("null");

			System.out.println();
		}
	
	}
	
}
