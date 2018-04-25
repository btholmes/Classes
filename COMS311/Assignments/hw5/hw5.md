Ben Holmes
ComS 311
HW 5

1.) 
ANSWER TO REMARK: 
Algorithm fails to produce smallest dominating set in situations where the smallest dominating set contains nodes which are adjacent to eachother. In this case, once the first node with the largest degree is found, it is removed, along with all its neighbors, which includes 
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
If you want to find an actual minimal dominating set, the algorithm can be rewritten in the following way. 
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


2.) 
ANSWER: 

Part 1.) Determine if S is independent
OVERVIEW: 
	Goes through each vertex in S, and for each vertex gets the set of all that vertex's edges. Then it loops through those edges
	and checks whether the given edge has already been found. If it has, return false. 

Input G = (V, E), S(subset of V)
boolean result = true
HashMap<Integer, Boolean> found = {}
	for each v in S
		edges = N(v) 		// All neighbors for given vertex
		for each e in edges
			if(found.containsKey(e)) 
				return false
			else 
				found.put(e, true)

Output result.

RUNTIME: 
	There are at most n iterations, and on each iteration we check at most m possible edges.
	O(nm)


Part 2.) Find a maximal independent set
Note: For the (isIndependent()) function, use the algorithm from part 1. 

Input G = (V, E)
M = {}
for vertex v in V
	if(isIndependent(M U {v}))
		M = M U {v}
Output M

RUNTIME: 
	Goes through at most n iterations, and at each iteration checks if it is independent, which requires nm checks. 
	Total time is O(n^2m)


3.) 
ANSWER: 

DYNAMIC (RECURSIVE): 

    getCost(i, j, a, b, c) 
        result = 0;
        cutA = 0;
        cutB = 0;
        cutC = 0;

        if(i == rows-1 && j == 0 && c == 1)
            return m[i][j];
        if(i == rows-1 && b == 1)
            return m[i][j];
        if(j == 0 && a == 1)
            return m[i][j];

        if(valid(i, j-1) && a == 1)
			cutA =  m[i][j] + getCost(i, j-1, 1, 0, 0);

        if(valid((i+1) , j) && b == 1) {
           cutB = m[i][j] + getCost(i+1, j, 0, 1, 0);
        }

		if(valid(i+1, j-1) && c == 1)
			cutC = m[i][j] + getCost(i+1, j-1, 0, 0, 1);

		t = Math.max(cutA, cutB);
		result = Math.max(t, cutC);

        return result;
   
RECURRENCE RELATION OF DYNAMIC: 
		T(n, m) = T(n+1, m) + T(n, m-1) + T(n+1, m-1)


ITERATIVE: 

    getCost(i, j){
        result = 0;
        cutA = 0;
        cutB = 0;
        cutC = 0;

        for(int k = j; k >= 0; k--){
            cutA += m[i][k];
        }

        for(int k = i; k < rows; k++){
            cutB += m[k][j];
        }

        int k = i;
        int l = j;
        while(k < rows && l >= 0){
            cutC += m[k][l];
            k++;
            l--;
        }

        t = Math.max(cutA, cutB);
        result = Math.max(t, cutC);
        return result;
    }


RUNTIME OF ITERATIVE: 
	O(m) for cut A + O(n) for cutB + O(n+m) for cutC, 
	thus total time is O(m + n + m+n) = O(n+m); 


4.) 

ANSWER: 

DYNAMIC (RECURSIVE): 

    recursive(set, subset, n){

        if(n < 0) {
            return  sum(set) == sum(subset);
        }

        //Shallow copies
        setA = set;
        setB = subset;

        setB[n] = setA[n]
        setA[n] = 0

        return recursive(setA, setB, n-1) || recursive(set, subset, n-1);
    }

RECURRENCE RELATION: 
	T(n) = T(n-1) + T(n-1) + C


ITERATIVE: 

    iterative(set, subset){

        result = false;
        stack = new Stack<>();
        n = set.size() -1;

        stack.push(n);
        stack.push(subset);
        stack.push(set);


        while(!stack.isEmpty()){
            originalA = stack.pop();
            originalB = stack.pop();
            n = stack.pop();

            //Shallow copies
            copyA = originalA;
            copyB = originalB;

            a = sum(originalA);
            b = sum(originalB);

            if(a == b) return true;

            if(n < 0) continue;

            copyB[n] = copyA[n]
            copyA[n] = 0
            stack.push(n-1);
            stack.push(copyB);
            stack.push(copyA);

            stack.push(n-1);
            stack.push(originalB);
            stack.push(originalA);
        }

        return result;
    }

RUNTIME: 
	for each iteration through the stack, 2 more iterations are created, and each runs till n = -1, or n = 0 for simplicity. 
	This take O(2^n). On each iteration the sum of the two sets is computed.. This takes O(N + N)
	Total runtime is O(2^n(N + N)) = O(2^n)



