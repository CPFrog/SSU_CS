package events;

import java.awt.Color;
import java.awt.Point;
import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.filechooser.FileNameExtensionFilter;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import layout.Base;

public class Utils {
	
	public static void newBase(Base base){
		base.getText().setText("");
		base.setMidp_a(new JPanel());
		
		base.getxTextField().setText("");
		base.getyTextField().setText("");
		base.getwTextField().setText("");
		base.gethTextField().setText("");
		base.getColorTextField().setText("");
	}
	
	
	public static void open(Base base){
		FileReader reader = new FileReader();
		
		JFileChooser fileChooser = new JFileChooser();
		FileNameExtensionFilter filter = new FileNameExtensionFilter("JSON ���� (*.json)", "json");
		fileChooser.setFileFilter(filter);
		
		int result = fileChooser.showOpenDialog(null); // null�� ������ �� ���α׷��� ���� ������ Ž�� ������

		if(result == JFileChooser.APPROVE_OPTION){
			File selectedFile = fileChooser.getSelectedFile();
			
			String fileName = selectedFile.getPath().toString();
			String resultStr = reader.getReader(fileName, "UTF-8");
			String mindText = loadStrConvert(resultStr);
			
			base.getText().setText(mindText);
			base.setLoadFileName(fileName);
			
		}
	}
	
	public static void save(Base base, String basefileName, int type){ //type1�̸� ����, type2�� �ٸ��̸���������
		JFrame window = new JFrame();
		FileWriter writer = new FileWriter();
		if(type==1){
			if(!basefileName.equals("")){//���� ��ο� ����
				
				writer.setTxtFile(basefileName, JsonConvert.JsonConvert(base.getText().getText()));
				
			}else{//���� ����
				JFileChooser fileChooser = new JFileChooser();
				FileNameExtensionFilter filter = new FileNameExtensionFilter("JSON ���� (*.json)", "json");
				fileChooser.setFileFilter(filter);
		         
		        //�������� ���̾�α� �� ���
		        int result = fileChooser.showSaveDialog(window);
		         
		        if (result == JFileChooser.APPROVE_OPTION) {
		            //������ ������ ��� ��ȯ
		            File selectedFile = fileChooser.getSelectedFile();
		            
		            String fileName = selectedFile.getPath().toString() + ".json";
		            System.out.println(fileName);
		            
		            writer.setTxtFile(fileName, JsonConvert.JsonConvert(base.getText().getText()));
		            base.setLoadFileName(fileName);
		            
		        }
			}
		}else{
			JFileChooser fileChooser = new JFileChooser();
			FileNameExtensionFilter filter = new FileNameExtensionFilter("JSON ���� (*.json)", "json");
			fileChooser.setFileFilter(filter);
	         
	        //�������� ���̾�α� �� ���
	        int result = fileChooser.showSaveDialog(window);
	         
	        if (result == JFileChooser.APPROVE_OPTION) {
	            //������ ������ ��� ��ȯ
	            File selectedFile = fileChooser.getSelectedFile();
	            
	            String fileName = selectedFile.getPath().toString() + ".json";
	            System.out.println(fileName);
	            
	            writer.setTxtFile(fileName, JsonConvert.JsonConvert(base.getText().getText()));
	            base.setLoadFileName(fileName);
	            
	        }
		}
	}
	
	
	public static void modify(Base base){
		String x 	= base.getxTextField().getText();
		x=x.trim();
		if(x.equals("")) {
			return;
		}
		
		String y	= base.getyTextField().getText();
		y=y.trim();
		
		if(y.equals("")) {
			return;
		}
		String width 	= base.getwTextField().getText();
		width=width.trim();
		
		if(width.equals("")) {
			return;
		}
		String height = base.gethTextField().getText();
		height=height.trim();
		
		if(height.equals("")) {
			return;
		}
		String colorStr = base.getColorTextField().getText();

		//x,y ��ǥ ����
		Point p = new Point(Integer.parseInt(x),Integer.parseInt(y));
		base.getClickNode().setLocation(p);
		
		//width, height setting
		base.getClickNode().setSize(Integer.parseInt(width), Integer.parseInt(height));
		
		String colorSplit [] = colorStr.split(",");
		
		int R = Integer.parseInt(colorSplit[0].trim());
		int G = Integer.parseInt(colorSplit[1].trim());
		int B = Integer.parseInt(colorSplit[2].trim());
		
		Color color = new Color(R,G,B);
		base.getClickNode().setBackground(color);
	}
	
	public static void apply(Base base){
		MindMapPaint mindMapPaint = new MindMapPaint();
		mindMapPaint.mindMapPaint(base, base.getText().getText());
		base.repaint();
	}
	
	public static String loadStrConvert(String resultStr){
		JSONParser parser = new JSONParser(); 
		Object obj;
		JSONObject jsonObj;
		String result = "";
		try {
			obj = parser.parse(resultStr);
			jsonObj = (JSONObject) obj;
			
			JSONArray depth1Array = (JSONArray) jsonObj.get("mindMap");
			for(int i = 0 ; i<depth1Array.size(); i++){
				JSONObject depth1Object = (JSONObject) depth1Array.get(i);
				result = result + depth1Object.get("text") + "\n";
				if(depth1Object.containsKey("children")){//������ �ִ��� Ȯ��
					JSONArray depth2Array = (JSONArray) depth1Object.get("children");
					for(int j = 0 ; j<depth2Array.size(); j++){
						JSONObject depth2Object = (JSONObject) depth2Array.get(j);
						result = result + "\t" + depth2Object.get("text") + "\n";
						
						if(depth2Object.containsKey("children")){//������ �ִ��� Ȯ��
							JSONArray depth3Array = (JSONArray) depth2Object.get("children");
							for(int z = 0 ; z<depth3Array.size(); z++){
								JSONObject depth3Object = (JSONObject) depth3Array.get(z);
								result = result + "\t" + "\t" + depth3Object.get("text")+ "\n";
							}
					    }
					}
			    }
			}
		} catch (ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		return result;
	}
}

