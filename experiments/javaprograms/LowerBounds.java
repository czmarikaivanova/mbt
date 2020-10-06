import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.PrintWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.Collections;
import java.util.stream.Stream;

public class LowerBounds {

	static File amplFile;
	static File objFile;
	static int nodeCnt;
	static int sourceCnt;
	static int maxDeg;
	
	public static void main(String[] args) {
		amplFile = new File(args[0]);
		objFile = new File(args[1]);
		maxDeg = 0;
		nodeCnt = findParam("param n ");
		sourceCnt = findParam("param s "); // the last space must be present due to param spg, which we don't consider here.
		int logB = logBound();
		int degB = degSecBound();
		int fibB = fibBound(nodeCnt, sourceCnt, maxDeg);
		printResToFile(logB, degB, fibB);
		int lb = Math.max(Math.max(logB,degB),fibB);
		System.exit(lb);
	}

	private static void printResToFile(int lb, int dsb, int fb) {
      		try {

      		   // create a new writer
      		   PrintWriter pw = new PrintWriter(new FileWriter(objFile, true));
      		   // printf text with default locale.
      		   // %s indicates a string will be placed there, which is s
      		   pw.printf("%d\t%d\t%d\t", lb,fb, dsb);
      		   // flush the writer
      		   pw.flush();

      		} catch (Exception ex) {
      		   ex.printStackTrace();
      		}
	}
	
	private static int logBound() {
		double frac = nodeCnt / sourceCnt;
		int lb =  (int) Math.ceil((Math.log(frac)/ Math.log(2)));
		//System.out.println("Logarithmic bound : " + lb);
		return lb;
		
	}
	
	private static int findParam(String paramStr) {
		int paramVal = 0;
		try (BufferedReader br = new BufferedReader(new FileReader(amplFile))) {
		    String line;
		    while ((line = br.readLine()) != null) {
		    	if (line.contains(paramStr)) {
		    		String[] words = line.split(" ");
		    		String lastWord = words[words.length - 1];
				//System.out.println(lastWord);
		    		String numStr = lastWord.substring(0, lastWord.length() - 1);
		    		paramVal = Integer.parseInt(numStr);
		    	}
		    }
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return paramVal;
	}
	
	private static int degSecBound() {
		Integer[] degrees = new Integer[nodeCnt - sourceCnt];
		Integer[] sourceDegrees = new Integer[sourceCnt];
		Integer[] both = null;
		for (int i = 0; i < sourceCnt; i++) {
			sourceDegrees[i] = 0;
		}
		for (int i = 0; i < nodeCnt-sourceCnt; i++) {
			degrees[i] = 0;
		}

		String edgeStr = "";
		boolean reached = false;
		try (BufferedReader br = new BufferedReader(new FileReader(amplFile))) {
		    String line;
		    while ((line = br.readLine()) != null) {
		    	if (line.contains("set E")) {
		    		reached = true;
		    	}
		    	if (reached) {
		    		edgeStr += line + " ";
		    	}
		    }
		String[] individualEdgesStr = edgeStr.split(" ");
		for (String eStr: individualEdgesStr) {
			if (eStr.contains("(")) { // it is an ege string and not 'param' etc
				int num = Integer.parseInt(eStr.substring(eStr.indexOf('(') + 1, eStr.indexOf(',')));
				if (num < sourceCnt) {
					sourceDegrees[num]++;
				}
				else {
					degrees[num - sourceCnt]++;
				}
				num = Integer.parseInt(eStr.substring(eStr.indexOf(',') + 1, eStr.indexOf(')')));
				if (num < sourceCnt) {
					sourceDegrees[num]++;
				}
				else {
					degrees[num - sourceCnt]++;
				}
			}
		}
		Arrays.sort(sourceDegrees, Collections.reverseOrder());
		Arrays.sort(degrees, Collections.reverseOrder());
		both = Stream.concat(Arrays.stream(sourceDegrees), Arrays.stream(degrees)).toArray(Integer[]::new);
		maxDeg = degrees[0];
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//System.out.println("sourceDegrees[i]");
		//for (Integer i: sourceDegrees) {
		//	System.out.println(i);
		//}
		//System.out.println("degrees[i]");
		//for (Integer i: degrees) {
		//	System.out.println(i);
		//}
		//System.out.println("both[i]");
		//for (Integer i: both) {
		//	System.out.println(i);
		//}
		int db =  degSec(both);
		//System.out.println("Degree bound: " + db);
		return db;
	}

	private static int degSec(Integer[] degs) {
		int[] A = new int[nodeCnt];
		for (int i = 0; i < nodeCnt; i++) {
			A[i] = 0;
		}
		int c = sourceCnt;
		int k = 0;
		//System.out.println("c: " + c + " nodeCnt: " + nodeCnt);
		while (c < nodeCnt) {
			k++;
			int cn = 0;
			for (int i = 0; i < c; i++) {
				if (A[i] < degs[i]) {
					A[i]++;
					cn++;
					if (c + cn - 1 < nodeCnt) {
						A[c + cn - 1]=1;
					}
				}
			}
			c += cn;
		}
		return k;
	}

	private static int fibBound(int n, int s, int d) {
		int[] fib = new int[n];
		fib[0] = 1;
		fib[1] = 1;
		int k = 1;
		while (2 * s * fib[k] < n) {
			k++;
			for (int i= 1; i <= d; i++) {
				fib[k] += (k - i  >= 0 ? fib[k-i] : 0);
			}
		}
	//	for (int i = 0; i < fib.length; i++) {
	//		System.out.println(i + ": " + fib[i]);
	//	}
//		int fb = (int) Math.ceil(k / s);
		int fb = k;
		//System.out.println("max deg: " + d);
		//System.out.println("Fibonacci bound: " + fb);
		return fb;
	}



}
