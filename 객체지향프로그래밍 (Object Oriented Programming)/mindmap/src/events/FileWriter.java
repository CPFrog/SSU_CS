package events;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

import org.json.simple.JSONObject;

public class FileWriter {
	
	/**
	 *  파일  writer
	 *  - filePath :filePath(파일이름 포함)
	 *  - JSONObject : json 파싱
	 **/
	public void setTxtFile(String filePath, JSONObject jsonObject){
		try{
			String setTxtFile = filePath;
			
			String bodyText = jsonObject.toJSONString();
			
			BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(setTxtFile),"UTF-8"));
			writer.write(bodyText);
			
			writer.close();
		}catch(IOException e){
			e.printStackTrace();
		}
	}
}
