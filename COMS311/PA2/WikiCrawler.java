// LEAVE THIS FILE IN THE DEFAULT PACKAGE
//  (i.e., DO NOT add 'package cs311.pa1;' or similar)

// DO NOT MODIFY THE EXISTING METHOD SIGNATURES
//  (you may, however, add member fields and additional methods)

// DO NOT INCLUDE LIBRARIES OUTSIDE OF THE JAVA STANDARD LIBRARY
//  (i.e., you may only include libraries of the form java.*)

/**
* @author Hugh Potter
*/

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;


public class WikiCrawler
{
	static final String BASE_URL = "https://en.wikipedia.org/";
	static String seedUrl; 
	static int max; 
	static ArrayList<String> topics; 
	static String fileName; 

	public WikiCrawler(String seedUrl, int max, ArrayList<String> topics, String fileName)
	{
		// implementation
		this.seedUrl = seedUrl; 
		this.max = max; 
		this.topics = topics; 
		this.fileName = fileName; 
	}

	public void crawl() throws IOException
	{
		// implementation
		URL url;
		try {
			url = new URL(BASE_URL+"/wiki/Physics");
			InputStream is = url.openStream();
			BufferedReader br = new BufferedReader(new InputStreamReader(is));
			readUrl(br); 			
			br.close();
		} catch (MalformedURLException e) {
			e.printStackTrace();
		}
		
	}
	
	private void readUrl(BufferedReader br) throws IOException {
	     String line = br.readLine(); 
	     File file = new File(fileName); 
         while(line != null){  

//       	  	line = line.replaceAll(" ", "")
//               .replaceAll("\t", "")
//               .replaceAll("\\.", "")
//               .replaceAll(",", "")
//               .replaceAll(":", "")
//               .replaceAll(";", "");
       	  	
       	  	line.toLowerCase(); 
       	  	writeToFile(line, file); 
       	  	line = br.readLine(); 
         }  
	}
	
	 private static void writeToFile(String data, File file) {
	        OutputStream os = null;
	        try {
	        		boolean append = true; 
	            os = new FileOutputStream(file, append);
	            os.write(data.getBytes(), 0, data.length());
	        } catch (IOException e) {
	            e.printStackTrace();
	        }finally{
	            try {
	                os.close();
	            } catch (IOException e) {
	                e.printStackTrace();
	            }
	        }
	    }
	
}