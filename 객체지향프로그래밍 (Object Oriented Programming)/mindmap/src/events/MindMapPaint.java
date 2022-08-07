package events;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.JPanel;

import layout.Base;
import layout.Midpane;

public class MindMapPaint {

	public void mindMapPaint(Base base, String text){
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
		
		
		JPanel midp_a = new Midpane(base, list);
		midp_a.setSize(600,480);

		base.midp.removeAll();
		base.midp.add(midp_a);
			
		
	}
}
