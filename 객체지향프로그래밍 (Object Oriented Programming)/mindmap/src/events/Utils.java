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
		FileNameExtensionFilter filter = new FileNameExtensionFilter("JSON 파일 (*.json)", "json");
		fileChooser.setFileFilter(filter);
		
		int result = fileChooser.showOpenDialog(null); // null로 지정시 이 프로그램의 저장 영역이 탐색 시작점

		if(result == JFileChooser.APPROVE_OPTION){
			File selectedFile = fileChooser.getSelectedFile();
			
			String fileName = selectedFile.getPath().toString();
			String resultStr = reader.getReader(fileName, "UTF-8");
			String mindText = loadStrConvert(resultStr);
			
			base.getText().setText(mindText);
			base.setLoadFileName(fileName);
			
		}
	}
	
	public static void save(Base base, String basefileName, int type){ //type1이면 저장, type2면 다른이름으로저장
		JFrame window = new JFrame();
		FileWriter writer = new FileWriter();
		if(type==1){
			if(!basefileName.equals("")){//기존 경로에 저장
				
				writer.setTxtFile(basefileName, JsonConvert.JsonConvert(base.getText().getText()));
				
			}else{//새로 저장
				JFileChooser fileChooser = new JFileChooser();
				FileNameExtensionFilter filter = new FileNameExtensionFilter("JSON 파일 (*.json)", "json");
				fileChooser.setFileFilter(filter);
		         
		        //파일저장 다이얼로그 를 띄움
		        int result = fileChooser.showSaveDialog(window);
		         
		        if (result == JFileChooser.APPROVE_OPTION) {
		            //선택한 파일의 경로 반환
		            File selectedFile = fileChooser.getSelectedFile();
		            
		            String fileName = selectedFile.getPath().toString() + ".json";
		            System.out.println(fileName);
		            
		            writer.setTxtFile(fileName, JsonConvert.JsonConvert(base.getText().getText()));
		            base.setLoadFileName(fileName);
		            
		        }
			}
		}else{
			JFileChooser fileChooser = new JFileChooser();
			FileNameExtensionFilter filter = new FileNameExtensionFilter("JSON 파일 (*.json)", "json");
			fileChooser.setFileFilter(filter);
	         
	        //파일저장 다이얼로그 를 띄움
	        int result = fileChooser.showSaveDialog(window);
	         
	        if (result == JFileChooser.APPROVE_OPTION) {
	            //선택한 파일의 경로 반환
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

		//x,y 좌표 세팅
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
				if(depth1Object.containsKey("children")){//하위단 있는지 확인
					JSONArray depth2Array = (JSONArray) depth1Object.get("children");
					for(int j = 0 ; j<depth2Array.size(); j++){
						JSONObject depth2Object = (JSONObject) depth2Array.get(j);
						result = result + "\t" + depth2Object.get("text") + "\n";
						
						if(depth2Object.containsKey("children")){//하위단 있는지 확인
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

