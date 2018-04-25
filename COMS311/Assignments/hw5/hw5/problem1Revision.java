package hw5;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Scanner;

public class problem1Revision {

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
		
		while(in.hasNextInt()) {
//			Node
			int a = in.nextInt(); 
//			Edge
			int b = in.nextInt(); 
			G.get(a).add(b); 
			G.get(b).add(a); 
		}

		
		System.out.println(getDominatingSet());
	}
	
	private static boolean isDominating(ArrayList<Integer> X) {
		HashMap<Integer, Boolean> visited = new HashMap(); 
		for(Integer v : X) {
			visited.put(v,  true); 
			for(Integer edge : G.get(v)) {
				if(!visited.containsKey(edge))
					visited.put(edge, true); 
			}
		}
		return visited.size() == G.size(); 
	}
	
	private static ArrayList<Integer> getDominatingSet() {
		HashMap<Integer, Boolean> D = new HashMap<Integer, Boolean>(V); 
		for(Integer v : V.keySet()) {
			HashMap<Integer, Boolean> smallerD = new HashMap<Integer, Boolean>(D); 
			smallerD.remove(v); 
			if(isDominating(new ArrayList<Integer>(smallerD.keySet()))) {
				D.remove(v); 
			}
		}
		
		return new ArrayList<Integer>(D.keySet()); 
	}
	
}
