Ben Holmes
ComS 311
HW 5

1.) 
ANSWER TO REMARK: 
Algorithm fails to produce smallest set in situations where all nodes in the Dominating set are also each others neighbors. 
In this case, once the first node with the largest degree is found, it is removed, along with all its neighbors, which includes 
nodes that should be members of the minimal dominating set. Thus every subsequent iteration will only look for the largest degree 
in what is left of the graph. This will fail to produce the actual smallest dominating set. 

GIVEN ALGORITHM: 
(a) Input G = (V, E).
(b) Set D = ∅
(c) While V is not empty do
• let v ∈ V be a vertex with largest degree.
• Add v to D.
• Remove v and all vertices in N(v) from G (and thus from V ).
(d) Output D.


IMPLEMENTATION: 
HashMaps were used as the data structure to allow O(1) search and remove. 
	
	static HashMap<Integer, ArrayList<Integer>> G; 
	static HashMap<Integer, Boolean> V; 
	static int nodes; 

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
	

RUNTIME OF GIVEN ALGORITHM: 
There are at most n iterations, and at each iteration we check at most n nodes to find the one with the largest degree. In this iteration, we then remove the node with the largest degree and all its edges from G, which would consist of at most n + m removals.
The running time is O(n * n * (n+m) which is O(n^2 * (n+m))


REVISION: 
If you want to truly find a dominating set, the algorithm can be rewritten in the following way. 
(a) Input G = (V, E).
(b) Set D = V
(c) for each v in V
• E = D - {v}
• if(E is still dominating) 
• D = E
(d) Output D.


IMPLEMENTATION: 
HashMaps again were used to allow O(1) search and remove. 

	static HashMap<Integer, ArrayList<Integer>> G; 
	static HashMap<Integer, Boolean> V; 
	static int nodes; 

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


RUNTIME OF REVISED ALGORITHM: 
There are at most n iterations, and at each iteration we consider at most n possible dominating sets, where we check for the existence of m edges. The runtime is then O(n^2 * m). 








