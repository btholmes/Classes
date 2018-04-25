package hw5;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Scanner;

public class problem1 {

	
	static HashMap<Integer, ArrayList<Integer>> G; 
	static HashMap<Integer, Boolean> V; 
	static int nodes; 
	
	
	public static void main(String[] args) throws FileNotFoundException {
		File input = new File("input.txt"); 
		Scanner in = new Scanner(input); 
		nodes = in.nextInt(); 
	
		V = new HashMap<Integer, Boolean>(); 
		G = new HashMap<>(); 
		for(int i = 0; i < nodes; i++) {
			G.put(i, new ArrayList<Integer>()); 
			V.put(i, true); 
		}
		
//		E = new HashMap<Integer, Boolean>(); 
		while(in.hasNextInt()) {
			int a = in.nextInt(); 
			int b = in.nextInt(); 
			G.get(a).add(b); 
			G.get(b).add(a); 
//			E.put(b, true); 
		}
		
		System.out.println(getDominatingSet());
		
	}
	

	private static int getLargestDegree() {
		int maxEdges = 0; 
		int result = 0; 

		Iterator it = G.entrySet().iterator(); 
		while(it.hasNext()) {
			Map.Entry entry = (Map.Entry)it.next(); 
			int edges = ((ArrayList<Integer>)entry.getValue()).size(); 
			if(edges > maxEdges) {
				maxEdges = edges; 
				result = (Integer)entry.getKey(); 
			}
		}
		return result; 
	}
	
//	Remove neighbors and v
	private static void removeNeighbors(int v) {
		ArrayList<Integer> neighbors = G.get(v); 
		for(Integer e : neighbors) {
			G.remove(e); 
			V.remove(e); 
		}
		G.remove(v); 
	}
	
	private static ArrayList<Integer> getDominatingSet(){
		HashMap<Integer, Boolean> D = new HashMap<Integer, Boolean>(); 
		
		while(V.size() > 0) {
			int largestDegree = getLargestDegree(); 
			removeNeighbors(largestDegree); 
			if(!D.containsKey(largestDegree))
				D.put(largestDegree, true); 
			
			V.remove((Integer)largestDegree); 
		}
		
		return new ArrayList<Integer>(D.keySet()); 
	}
	
	
	
}
