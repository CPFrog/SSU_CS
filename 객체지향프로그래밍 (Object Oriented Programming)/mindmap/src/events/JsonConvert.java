package events;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.JPanel;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import layout.Base;
import layout.Midpane;

public class JsonConvert{
	/* �ؽ�Ʈ json ��ȯ
	 * */
	public static JSONObject JsonConvert(String text){
		String inputText = text;
		String[] inputTextSplit = inputText.split("\n");//1�پ� �ڸ���

		
		List<Map<String,Object>> list = new ArrayList<>();
		
		int depth1Id = 0;
		int depth2Id = 0;
		int depth3Id = 0;
		
		for(int i =0 ; i< inputTextSplit.length; i++){
			String line = inputTextSplit[i];
			
			String[] lineSplit = line.split("\t");
			
			int depth = lineSplit.length;//split���� \t�߶�, 1�̸� depth1, 2�̸� depth2, 3�̸� depth3
			
			Map<String,Object> map = new HashMap<String,Object>();
			
			String jsonText = lineSplit[depth-1];
			
			map.put("text", jsonText);
			map.put("depth", depth);
			
			if(depth == 1) {
				depth1Id++;
				map.put("id", String.valueOf(depth1Id) + "0" + "0");
				map.put("parentId", "");//�ֻ���̹Ƿ� �θ� Id�� ����.
				depth2Id = 0;
				depth3Id = 0;
			}else if(depth ==2) {
				depth2Id++;
				map.put("id", String.valueOf(depth1Id) + String.valueOf(depth2Id) + "0");
				map.put("parentId", String.valueOf(depth1Id) + "0" + "0");//depth1�� ���̵�
			}else if(depth ==3) {
				depth3Id++;
				map.put("id", String.valueOf(depth1Id) + String.valueOf(depth2Id) + String.valueOf(depth3Id));
				map.put("parentId", String.valueOf(depth1Id) + String.valueOf(depth2Id) + "0");//depth2�� ���̵�
			}
			list.add(map);
		}
//		System.out.println(list.toString());
		
		//���� �ϼ��� JSONObject ����(��ü)
        JSONObject jsonObject = new JSONObject();
		
        
        //depth1�� �ش�Ǵ� depth1Array����
        JSONArray depth1 = new JSONArray();
		for(int i = 0 ; i <list.size(); i++) {
			if(list.get(i).get("depth").toString().equals("1")) {
				//depth1�� JsonObject
				JSONObject depth1Object = new JSONObject();
				depth1Object.put("text",list.get(i).get("text").toString());
				depth1Object.put("id", list.get(i).get("id").toString());
				depth1Object.put("parentId", list.get(i).get("parentId").toString());
				depth1Object.put("depth", 1);
				
				//depth1�� ���� depth2Array����
		        JSONArray depth2 = new JSONArray();
				for(int j=0; j<list.size() ; j++) {//depth2 ����
					if(list.get(i).get("id").toString().equals(list.get(j).get("parentId").toString()) && list.get(j).get("depth").toString().equals("2")){
						
						//depth2�� JsonObject
						JSONObject depth2Object = new JSONObject();
						depth2Object.put("text",list.get(j).get("text").toString());
						depth2Object.put("id", list.get(j).get("id").toString());
						depth2Object.put("parentId", list.get(j).get("parentId").toString());
						depth2Object.put("depth", 2);
						
						//depth2�� ���� depth2Array����
				        JSONArray depth3 = new JSONArray();
						for(int z =0; z<list.size(); z++) {//depth3 ����
							if(list.get(j).get("id").toString().equals(list.get(z).get("parentId").toString()) 
									&& list.get(z).get("depth").toString().equals("3")){
								//depth3�� JsonObject
								JSONObject depth3Object = new JSONObject();
								depth3Object.put("text",list.get(z).get("text").toString());
								depth3Object.put("id", list.get(z).get("id").toString());
								depth3Object.put("parentId", list.get(z).get("parentId").toString());
								depth3Object.put("depth", 3);
							
								depth3.add(depth3Object);
								depth2Object.put("children", depth3);
							}
							
						}
						depth2.add(depth2Object);
						depth1Object.put("children", depth2);
					}
				}
				depth1.add(depth1Object);
				
			}
		}
		
		jsonObject.put("mindMap", depth1);
		//System.out.println(jsonObject.get("depth1"));
		
		return jsonObject;
	}
	

}
