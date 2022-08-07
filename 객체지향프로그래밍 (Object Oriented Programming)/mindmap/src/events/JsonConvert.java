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
	/* 텍스트 json 변환
	 * */
	public static JSONObject JsonConvert(String text){
		String inputText = text;
		String[] inputTextSplit = inputText.split("\n");//1줄씩 자르기

		
		List<Map<String,Object>> list = new ArrayList<>();
		
		int depth1Id = 0;
		int depth2Id = 0;
		int depth3Id = 0;
		
		for(int i =0 ; i< inputTextSplit.length; i++){
			String line = inputTextSplit[i];
			
			String[] lineSplit = line.split("\t");
			
			int depth = lineSplit.length;//split으로 \t잘라서, 1이면 depth1, 2이면 depth2, 3이면 depth3
			
			Map<String,Object> map = new HashMap<String,Object>();
			
			String jsonText = lineSplit[depth-1];
			
			map.put("text", jsonText);
			map.put("depth", depth);
			
			if(depth == 1) {
				depth1Id++;
				map.put("id", String.valueOf(depth1Id) + "0" + "0");
				map.put("parentId", "");//최상단이므로 부모 Id가 없다.
				depth2Id = 0;
				depth3Id = 0;
			}else if(depth ==2) {
				depth2Id++;
				map.put("id", String.valueOf(depth1Id) + String.valueOf(depth2Id) + "0");
				map.put("parentId", String.valueOf(depth1Id) + "0" + "0");//depth1의 아이디
			}else if(depth ==3) {
				depth3Id++;
				map.put("id", String.valueOf(depth1Id) + String.valueOf(depth2Id) + String.valueOf(depth3Id));
				map.put("parentId", String.valueOf(depth1Id) + String.valueOf(depth2Id) + "0");//depth2의 아이디
			}
			list.add(map);
		}
//		System.out.println(list.toString());
		
		//최종 완성될 JSONObject 선언(전체)
        JSONObject jsonObject = new JSONObject();
		
        
        //depth1에 해당되는 depth1Array선언
        JSONArray depth1 = new JSONArray();
		for(int i = 0 ; i <list.size(); i++) {
			if(list.get(i).get("depth").toString().equals("1")) {
				//depth1의 JsonObject
				JSONObject depth1Object = new JSONObject();
				depth1Object.put("text",list.get(i).get("text").toString());
				depth1Object.put("id", list.get(i).get("id").toString());
				depth1Object.put("parentId", list.get(i).get("parentId").toString());
				depth1Object.put("depth", 1);
				
				//depth1에 들어가는 depth2Array선언
		        JSONArray depth2 = new JSONArray();
				for(int j=0; j<list.size() ; j++) {//depth2 추출
					if(list.get(i).get("id").toString().equals(list.get(j).get("parentId").toString()) && list.get(j).get("depth").toString().equals("2")){
						
						//depth2의 JsonObject
						JSONObject depth2Object = new JSONObject();
						depth2Object.put("text",list.get(j).get("text").toString());
						depth2Object.put("id", list.get(j).get("id").toString());
						depth2Object.put("parentId", list.get(j).get("parentId").toString());
						depth2Object.put("depth", 2);
						
						//depth2에 들어가는 depth2Array선언
				        JSONArray depth3 = new JSONArray();
						for(int z =0; z<list.size(); z++) {//depth3 추출
							if(list.get(j).get("id").toString().equals(list.get(z).get("parentId").toString()) 
									&& list.get(z).get("depth").toString().equals("3")){
								//depth3의 JsonObject
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
