package layout;

import java.awt.Color;
import java.awt.Point;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.border.CompoundBorder;
import javax.swing.border.EtchedBorder;
import javax.swing.event.MouseInputListener;

public class Node extends JLabel {
	Random r = new Random();
	
	Base base;
	
	//마우스 오프셋좌표
	int offX, offY;
	
	Midpane Midpane;
	
	List<Map<String,Object>> nodeList;
	
	public Node(Base base, String name, int depth, Midpane midpane, String id) {
		this.base = base;
		this.Midpane = midpane;
		
		
		setText(name.trim());
		
		int x = 0; //위치값 0으로 초기화
		int y = 0; //위치값 0으로 초기화
		
		Random random = new Random();
		
		int xPlusToggle = random.nextInt(2) + 1; //1~2의 랜덤
		
		//위치값 오차주기위해서
		int xDev = random.nextInt(50) + 30; //50~100의 랜덤
		
		setOpaque(true);
		setBorder(new CompoundBorder((new EtchedBorder(EtchedBorder.RAISED)),
				BorderFactory.createEmptyBorder(10, 10, 10, 10)));
		if(depth == 1) {
			if(!midpane.depth1Be) {
				x = 240;
				y = 100;
				
				midpane.setDepth1Be(true);
			}else {
				if(xPlusToggle == 1) {
					x = x + xDev;
				}else {
					x = x - xDev;
				}
				
				y=100;
				
			}
			setBackground(Color.RED);
		}else if(depth == 2) {
			if(xPlusToggle == 1) {
				x = random.nextInt(Midpane.getWidth()-100) + xDev;
			}else {
				x = random.nextInt(Midpane.getWidth()-100) - xDev;
			}
			
			y=250;
			
			setBackground(Color.ORANGE);
			
			
		}else if(depth == 3) {
			if(xPlusToggle == 1) {
				x = random.nextInt(Midpane.getWidth()-100) + xDev;
			}else {
				x = random.nextInt(Midpane.getWidth()-100) - xDev;
			}
			
			y=400;
			setBackground(Color.YELLOW);
		}
		
		System.out.println(x + "," +y);
		
		Point p = new Point(x,y);
		
		setName(id);
//		setBackground(Color.white);
		setSize(95,50);
		setLocation(p);
		addMouseListener(new MouseEvent(this));
		
		
	}

	class MouseEvent implements MouseListener, MouseInputListener{

		Node node;
		Color tempc;
		Boolean pressed = false;
		
		MouseEvent(Node node){
			this.node = node;
			
		}
		
		@Override
		public void mouseDragged(java.awt.event.MouseEvent e) {}

		@Override
		public void mouseMoved(java.awt.event.MouseEvent e) {}

		@Override
		public void mouseClicked(java.awt.event.MouseEvent e) {
			base.textatb.setText(node.getText());
			base.xTextField.setText(String.valueOf(node.getX()));
			base.yTextField.setText(String.valueOf(node.getY()));
			base.wTextField.setText(String.valueOf(node.getWidth()));
			base.hTextField.setText(String.valueOf(node.getHeight()));
			
			String red 		= String.valueOf(node.getBackground().getRed());
			String green 	= String.valueOf(node.getBackground().getGreen());
			String blue 	= String.valueOf(node.getBackground().getBlue());
			base.colorTextField.setText(red + ","+green+","+blue);



		}

		@Override
		public void mousePressed(java.awt.event.MouseEvent e) {
			
			if(node != null) {
				offX = e.getX() - node.getX();
				offY = e.getY() - node.getY();
				
				pressed = true;
				base.clickNode = node;
				tempc = base.clickNode.getBackground();
				base.clickNode.setBackground(Color.CYAN);
			}
			
		}

		@Override
		public void mouseReleased(java.awt.event.MouseEvent e) {
			//x,y 좌표 세팅
			int x = e.getX() - offX;
			int y = e.getY() - offY;
			
			Point p = new Point(x,y);
			if(base.clickNode != null) {
				base.clickNode.setLocation(p);
				
				List<Map<String,Object>> getNodeList = Midpane.getNodeList();
				
				for(int i =0 ; i<getNodeList.size(); i++){
					if(getNodeList.get(i).get("id").equals(base.clickNode.getName())){
						Node node = (Node) getNodeList.get(i).get("node");
						
						Map<String, Object> map = new HashMap();
						map.put("node", node);
						map.put("id", getNodeList.get(i).get("id").toString());
						map.put("parentId", getNodeList.get(i).get("parentId").toString());
						map.put("depth", getNodeList.get(i).get("depth").toString());
						map.put("x", node.getX());
						map.put("y", node.getY());
						map.put("width", node.getWidth());
						map.put("height", node.getHeight());
						map.put("color", node.getBackground());
						
						getNodeList.remove(i);
						
						getNodeList.add(map);
					}
				}
				
				Midpane.setNodeList(getNodeList);
				Midpane.paintChildren(getGraphics());
			}
		}

		@Override
		public void mouseEntered(java.awt.event.MouseEvent e) {
			base.clickNode = node;
		}

		@Override
		public void mouseExited(java.awt.event.MouseEvent e) {
			if (pressed == true) {
				base.clickNode.setBackground(tempc);
				pressed = false;
			}
		}
	}
	
	
	public List<Node> changeNodeList(Node node, List<Node> nodeList){
		List<Node> result = new ArrayList<>();
		
		
		return result;
	}
	
}