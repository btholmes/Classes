import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;

public class main {
	
	
	public static void main(String[] args)  {
		ArrayList<String> topics = new ArrayList<String>(Arrays.asList("Iowa State", "Cyclones")); 
		WikiCrawler w = new WikiCrawler("/wiki/Iowa_State_University", 100, topics, "WikiISU.txt");
		try {
			w.crawl();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.out.println("Exception");
		}
	
	}
	
	private void testFileCreation() throws FileNotFoundException{
		File file = new File("random.txt"); 
		OutputStream os = new FileOutputStream(file);
        try {
        		String data = "Hello"; 
			os.write(data.getBytes(), 0, data.length());
			os.write("\n".getBytes());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.out.println("Exception");
		}
	}

}
