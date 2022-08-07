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
	 * ����� ������ �����͸� �����´�
	 * parameter 
	 * -path : ���ϰ��
	 * -encoding: encoding Ÿ�� 
	 **/
	public String getReader(String path, String encoding) {
		List<Map<String,Object>> result = new ArrayList();
		
		BufferedReader br = null;
		String text = "";
		String line;
		try {
			//������ reading
			br = new BufferedReader(new InputStreamReader(new FileInputStream(path), encoding));
			while ((line = br.readLine()) != null) {//���پ� �о��
				
				text = text + line;
			}
		} catch (FileNotFoundException e) {//������ ������,
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
