
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Random;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Map.Entry;


public class RandomGraph {

	static final int defaultK = 6;
	static boolean[][] M;
	static int s; // # of source sats
	static int n;
	static float delta;
	static int id;
	static Random rnd;
	static String dest;
	/**
	 * Creates graph from an input file
	 * @param fName input file
	 */

	public static void main(String[] args) throws IOException {
		if (args.length == 5) { // 4 args for generating a random graph
										// (# iteration, # nodes, # sources,
										// density of edges)
			int iter = Integer.parseInt(args[0]);
			n = Integer.parseInt(args[1]);
			s = Integer.parseInt(args[2]);
			delta = Float.parseFloat(args[3]);
			dest = args[4];
			if (dest.charAt(dest.length() - 1) == '*') {
				dest = dest.substring(0,dest.length() - 1);
			}
			System.out.println("dest: " + dest);
			for (int i = 0; i < iter; i++) {
				createGraph();
				String amplFileName = generateAMPL(shortestPaths());
			}
		} else {
			throw new IOException("Incorrect number of arguments ");
		}

	}
	
	/**
	 * Generates random graph with n nodes including an artificial source zero that does not appear in the data file
	 * @param n # of nodes including root
	 * @param s # of source nodes
	 * @param delta density of edges
	 */
	private static void createGraph() {
		rnd = new Random();
		id = rnd.nextInt(100000);
		M = new boolean[n][n];
		ArrayList<ArrayList<Integer>> sets = new ArrayList<ArrayList<Integer>>();
//		ArrayList<Integer> firstSet = new ArrayList<Integer>();
//		sets.add(firstSet);
		for (int i = 0; i < n; i ++) {  // init sets
//			if (i < s) {
//				firstSet.add(i);
//				//if (i > 0) {
//					M[0][i] = true;
//				//}
//			}
//			else {
				ArrayList<Integer> set =  new ArrayList<Integer>();
				set.add(i);
				sets.add(set);
//			}
		}
		while (sets.size() > 1) {
			int u = rnd.nextInt(n);
			int v = rnd.nextInt(n);
			ArrayList<Integer> Lu = findSet(sets, u);
			ArrayList<Integer> Lv = findSet(sets, v);
			if (Lu != Lv) {
				merge(sets,Lu,Lv);
				M[u][v] = true;
				M[v][u] = true;
			}
		}
		// add remaining edges according to given density delta
		for (int i = 0; i < n; i++) {
			for (int j = i+1; j < n; j++) {
				if (!M[i][j]) {
					float rndF = rnd.nextFloat();
					if (rndF < delta) {
						M[i][j] = true;
						M[j][i] = true;
					}
				}
				
			}
		}
	}

	public int getN() {
		return n;
	}
	
	public int getS() {
		return s;
	}
	
	// Kruskal merge
	private static void merge(ArrayList<ArrayList<Integer>> sets,ArrayList<Integer> lu, ArrayList<Integer> lv) {
		sets.remove(lu);
		sets.remove(lv);
		lu.addAll(lv);
		sets.add(lu);
	}

	// Find which component a vertex u belongs to
	private static ArrayList<Integer> findSet(ArrayList<ArrayList<Integer>> sets, int u) {
		for (ArrayList<Integer> set: sets) {
			if (set.contains(u)) {
				return set;
			}
		}
		return null;
	}

	public String toString() {
		String str = "   ";
		for (int i = 0; i < M.length; i++) {
			str += i + " ";
		}
		for (int i = 0; i < M.length; i++) {
			str += "\n" + i + "  ";
			for (int j = 0; j < M[i].length; j++) {
				if (M[i][j]) {
					str += "1 ";
				}
				else {
					str += "0 ";
				}
			}
		}
		return str;
	}
	
	private static String generateAMPL(int[][] dstsFromSources) {
        try  {
		System.out.println(dest);
        	int fileNumber = new File(dest).list().length + 1;
        	String dataFileStr = dest + "/ampl-" + n + "-" + s + "-" +(int) (delta * 1000) + "-" + fileNumber  + ".dat";
        	File datafile = new File(dataFileStr);
            System.out.println("Saving: AMPL input");
            FileWriter fw = new FileWriter(datafile,false); //the true will append the new data
            fw.write("# " + id + "\n");
            int nodeCnt = n; // number of nodes without the artificial source 0;
            double fraction = nodeCnt/s;
            //int paramK = (int) Math.ceil(Math.log(fraction)/Math.log(2));
            //fw.write("param k := " + paramK + ";\n");
            fw.write("param n := " + nodeCnt + ";\n");
            fw.write("param s := " + s + ";\n");
           // fw.write("param spg := \n");
           // for (int i = 0; i < dstsFromSources.length; i++) {
           // 	for (int j = 0; j < dstsFromSources[i].length; j++) {
           // 		fw.write(i + " " + j + " " + dstsFromSources[i][j] + "\t");
           // 	}
           // 	fw.write("\n" + i + " " + n + " 1\n"); // distance from source to universal node
           // 	fw.write("\n");
           // }
           // fw.write(";\n");
            fw.write("set E := ");
        	int edgeCnt = 0;
        	for (int i = 0; i < n; i ++) {
            	for (int j = i + 1; j < n; j++) {
            		if (M[i][j]) {
            			fw.write(" (" + i + "," + j + ")");
            			edgeCnt++;
            		}
            	}
            }
            fw.write(";\n");
            fw.close();
            System.out.println("Edge count: " + edgeCnt);
            return dataFileStr;
        }
        catch(IOException ioe) {
            System.err.println("IOException: " + ioe.getMessage());
            return null;
        } 
	}

	public static int[][] shortestPaths() {
		int[][] distancesFromSources = new int[s][n];
		for (int i = 0; i < s; i++) {
			distancesFromSources[i] = dijkstra(i);
		}
	//	for (int i = 0; i < s; i++) {
	//		System.out.println("Source: " + i);
	//		for (int j = 0; j < n; j++) {
	//			System.out.println("Node: " + j + " distance: " + distancesFromSources[i][j]);
	//		}
	//	}
		return distancesFromSources;
	}
	
	public static int[] dijkstra(int s) {
		int[] distances = new int[n];
		int[] predecessors = new int[n];
		
		LinkedList<Integer> q = new LinkedList<>();
		for (int i = 0; i < n; i++) {
			q.add(i);
			if (i == s) {
				distances[s] = 0;
				predecessors[s] = -1;
			}
			else {
				distances[i] = 1000000;
				predecessors[s] = -1;
			}
		}
		while (!q.isEmpty()) {
			Integer u = extractMin(q, distances);
			q.remove(u);
			for (int v = 0; v < n; v++) {
				if (M[u][v]) {
					int alt = distances[u] + 1;
					if (alt < distances[v]) {
						distances[v] = alt;
						predecessors[v] = u;
					}
				}
			}
		}
		return distances;
	}
	
	private static Integer extractMin(LinkedList<Integer> q, int[] dsts) {
		int minDst = 1000;
		Integer minNode = -1;
		for (Integer i: q) {
			int value = dsts[i];
			if (value < minDst) {
				minDst = value;
				minNode = i;
			}
		}
			return minNode;
	}
	private static boolean isAdjacent(int i, int j) {
		return M[i][j];
	}
	
}
