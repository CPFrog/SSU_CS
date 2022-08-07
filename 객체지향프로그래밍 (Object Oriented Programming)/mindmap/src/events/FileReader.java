package events;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class FileReader {

	/**
	 * 저장된 파일의 데이터를 가져온다
	 * parameter 
	 * -path : 파일경로
	 * -encoding: encoding 타입 
	 **/
	public String getReader(String path, String encoding) {
		List<Map<String,Object>> result = new ArrayList();
		
		BufferedReader br = null;
		String text = "";
		String line;
		try {
			//데이터 reading
			br = new BufferedReader(new InputStreamReader(new FileInputStream(path), encoding));
			while ((line = br.readLine()) != null) {//한줄씩 읽어옴
				
				text = text + line;
			}
		} catch (FileNotFoundException e) {//파일이 없으면,
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (br != null) {
				try {
					br.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

		return text;
	}

}
