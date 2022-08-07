package layout;

import java.awt.Graphics;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.JLabel;
import javax.swing.JPanel;

public class Midpane extends JPanel {

	Base base;
	
	List<Map<String,Object>> nodeList;
	
	Boolean depth1Be = false;
	
	public Midpane(Base base) {
		this.base = base;
		setSize(600, 480);
	
		this.setLayout(null);
	}
	
	
	public Midpane(Base base, List<Map<String,Object>> list) {
		nodeList = new ArrayList<>();
		
		setSize(600, 480);
		
		this.setLayout(null);
		for(int i =0 ; i<list.size(); i++){
			Map<String,Object> map = new HashMap<>();
			Node node = new Node(base
								, list.get(i).get("text").toString()
								, Integer.parseInt(list.get(i).get("depth").toString())
								, this
								, list.get(i).get("id").toString());
			this.add(node);
			map.put("node", node);
			map.put("id", list.get(i).get("id").toString());
			map.put("parentId", list.get(i).get("parentId").toString());
			map.put("depth", list.get(i).get("depth").toString());
			map.put("x", node.getX());
			map.put("y", node.getY());
			map.put("width", node.getWidth());
			map.put("height", node.getHeight());
			map.put("color", node.getBackground());
			
			nodeList.add(map);
		}
		
		// 테두리 주기
		// https://blog.naver.com/PostView.nhn?blogId=blurzone&logNo=60017435787&proxyReferer=https%3A%2F%2Fwww.google.co.kr%2F
		// 공백 주기
		// http://unikys.tistory.com/210
	
		
	}
	
	public void paintChildren(Graphics g){
		super.paintChildren(g);
		
		if(nodeList != null){
			for(int i =0 ; i<nodeList.size(); i++){
				if(nodeList.get(i).get("depth").equals("1")){//1번 뎁스이면
					int x1 = (int) nodeList.get(i).get("x");
					int y1 = (int) nodeList.get(i).get("y");
//					int width1 = (int) nodeList.get(i).get("width");
//					int height1 = (int) nodeList.get(i).get("height");
					
					for(int j =0 ; j<nodeList.size(); j++){
						if(nodeList.get(j).get("parentId").equals(nodeList.get(i).get("id")) && nodeList.get(j).get("depth").equals("2")){
							int x2 = (int) nodeList.get(j).get("x");
							int y2 = (int) nodeList.get(j).get("y");
//							int width2 = (int) nodeList.get(j).get("width");
//							int height2 = (int) nodeList.get(j).get("height");
							
							g.drawLine(x1, y1, x2, y2);
							
							
							for(int z =0 ; z<nodeList.size(); z++){
								if(nodeList.get(z).get("parentId").equals(nodeList.get(j).get("id")) && nodeList.get(z).get("depth").equals("3")){
									int x3 = (int) nodeList.get(z).get("x");
									int y3 = (int) nodeList.get(z).get("y");
//									int width3 = (int) nodeList.get(z).get("width");
//									int height3 = (int) nodeList.get(z).get("height");
									g.drawLine(x2, y2, x3, y3);
								}
							}
						}
					}
				}
			}
		}
		
		repaint();
	}


	public List<Map<String, Object>> getNodeList() {
		return nodeList;
	}


	public void setNodeList(List<Map<String, Object>> nodeList) {
		this.nodeList = nodeList;
	}


	public Boolean getDepth1Be() {
		return depth1Be;
	}


	public void setDepth1Be(Boolean depth1Be) {
		this.depth1Be = depth1Be;
	}
	
	
	
}