package layout;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.GridLayout;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.event.MouseInputListener;

import events.MindMapPaint;
import events.Utils;

public class Base extends JFrame {

	Base base;
	
	//마인드맵 입력 텍스트 필드
	JTextArea text;
	
	//이름 텍스트필드
	JTextField textatb;
	
	//x좌표 필드
	JTextField xTextField;
	
	//y좌표 필드
	JTextField yTextField;
	
	//width 필드
	JTextField wTextField;
	
	//height 필드
	JTextField hTextField;
	
	//color필드
	JTextField colorTextField;
	
	Node clickNode;
	
	JPanel midp_a;
	
	public JPanel midp;
	
	
	String loadFileName = "";
	
	public Base() {
		
		this.base = this;
		
		setTitle("마인드맵");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // 창 닫으면 자동 프로그램 종료
		Container c = getContentPane(); // 전체 판
		Color bg_c = new Color(225,230,246);
		
		////////////// 좌측 패널 항목 //////////////
		JPanel leftp = new JPanel(); // left panel
		leftp.setBackground(bg_c);
		text = new JTextArea("", 26, 16);// 왼쪽 항목 텍스트 영역 및 처음 크기 지정
		text.setTabSize(3); // 탭하면 너무 멀리 가서 글자 3 길이만큼으로 변경
		JScrollPane lscroll = new JScrollPane(text); // text에 ScrollPane 부착 
		leftp.setLayout(new BorderLayout(0, 5)); // 레이아웃 변경, 세로로 5px 만큼 여백
		leftp.add(lscroll, BorderLayout.CENTER); // 항목 텍스트 영역이 부착된 lscroll을 leftp에 부착
		JButton txapplybtn = new JButton("적용"); // TeXt APPLY BuTtoN, 입력한 항목 적용 버튼 생성 
		txapplybtn.addMouseListener(new applyBtn());	
			
		
		leftp.add(txapplybtn, BorderLayout.SOUTH); // 항목 적용 버ㅇ튼의 위치 = 아래
		JLabel lefttitle = new JLabel("Text Editor Pane"); // 왼쪽 패널 이름 지정
		leftp.add(lefttitle, BorderLayout.NORTH); // 패널 이름 표시 위치 = 상단
		
		////////////// 우측 패널 항목 ////////////// 
		JPanel rightp = new JPanel();// 오른쪽 프레임 위 판 생성
		rightp.setBackground(bg_c);
		rightp.setLayout(new BorderLayout());// 오른쪽 판 배치관리자 지정
		JLabel rightatLabel = new JLabel("Attribute Pane");
		rightp.add(rightatLabel, BorderLayout.NORTH);// 라벨 위쪽 부착
		JPanel atpane = new JPanel();// 텍스트 입력 창 올리기 위한 판 부착
		atpane.setBackground(bg_c);
		rightp.add(atpane, BorderLayout.CENTER);// 판 위치 센터 지정
		
		GridLayout grid = new GridLayout(6, 2, 2, 20);// 텍스트 입력 창 올리기 위한 판의 배치관리자 지정. 그리드
		atpane.setLayout(grid);

		atpane.add(new JLabel("   TEXT: "));
		textatb = new JTextField(20); // 창의 열 개수 20
		textatb.setEditable(false); // 입력 못받는 JTextField로 변경
		atpane.add(textatb); // 오른쪽 attribute pane에 부착
		atpane.add(new JLabel("   X: "));
		xTextField = new JTextField(20);
		atpane.add(xTextField); // 창의 열 개수 20
		
		atpane.add(new JLabel("   Y: "));
		yTextField = new JTextField(20);
		atpane.add(yTextField); // 창의 열 개수 20
		atpane.add(new JLabel("   W: "));
		wTextField = new JTextField(20);
		atpane.add(wTextField); // 창의 열 개수 20
		atpane.add(new JLabel("   H: "));
		hTextField = new JTextField(20);
		atpane.add(hTextField); // 창의 열 개수 20
		atpane.add(new JLabel("   Color: "));
		colorTextField = new JTextField(20);
		atpane.add(colorTextField); // 창의 열 개수 20
		
		JButton atapplybtn = new JButton("변경");// 위치 지정 필요함
		
		atapplybtn.addMouseListener(new nodeChangeBtn());
		rightp.add(atapplybtn, BorderLayout.SOUTH);
		
		///////// 중앙 패널 //////////
		midp = new JPanel();
		midp.setLayout(null);
		midp_a = new Midpane(this);
		midp.add(midp_a);
		

		JSplitPane pane1 = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, midp, rightp);
		pane1.setDividerLocation(750);//첫 스플릿 지정
		JSplitPane pane2 = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, leftp, pane1);

		MenuBar menu = new MenuBar(this);
		ToolBar tool = new ToolBar(this);
		setJMenuBar(menu);
		c.add(tool, BorderLayout.NORTH);
		c.add(pane2);

		this.setSize(1024, 640);
		this.setVisible(true);

		// SplitPane 위치 지정
		pane1.setDividerLocation(.7); 
		pane2.setDividerLocation(.2);
	}
	
	class applyBtn implements MouseListener, MouseInputListener{

		@Override
		public void mouseDragged(MouseEvent e) {}

		@Override
		public void mouseMoved(MouseEvent e) {}

		@Override
		public void mouseClicked(MouseEvent e) {
			Utils.apply(base);
		}

		@Override
		public void mousePressed(MouseEvent e) {}

		@Override
		public void mouseReleased(MouseEvent e) {}

		@Override
		public void mouseEntered(MouseEvent e) {}

		@Override
		public void mouseExited(MouseEvent e) {}
	}
	
	class nodeChangeBtn implements MouseListener, MouseInputListener{

		@Override
		public void mouseDragged(MouseEvent e) {}

		@Override
		public void mouseMoved(MouseEvent e) {}

		@Override
		public void mouseClicked(MouseEvent e) {
			Utils.modify(base);
		}

		@Override
		public void mousePressed(MouseEvent e) {}

		@Override
		public void mouseReleased(MouseEvent e) {}

		@Override
		public void mouseEntered(MouseEvent e) {}

		@Override
		public void mouseExited(MouseEvent e) {}
	}

	
	public JTextArea getText() {
		return text;
	}


	public void setText(JTextArea text) {
		this.text = text;
	}


	public JTextField getTextatb() {
		return textatb;
	}


	public JTextField getxTextField() {
		return xTextField;
	}


	public JTextField getyTextField() {
		return yTextField;
	}


	public JTextField getwTextField() {
		return wTextField;
	}


	public JTextField gethTextField() {
		return hTextField;
	}


	public JTextField getColorTextField() {
		return colorTextField;
	}


	public void setTextatb(JTextField textatb) {
		this.textatb = textatb;
	}


	public void setxTextField(JTextField xTextField) {
		this.xTextField = xTextField;
	}


	public void setyTextField(JTextField yTextField) {
		this.yTextField = yTextField;
	}


	public void setwTextField(JTextField wTextField) {
		this.wTextField = wTextField;
	}


	public void sethTextField(JTextField hTextField) {
		this.hTextField = hTextField;
	}


	public void setColorTextField(JTextField colorTextField) {
		this.colorTextField = colorTextField;
	}


	public Node getClickNode() {
		return clickNode;
	}


	public void setMidp_a(JPanel midp_a) {
		this.midp_a = midp_a;
	}


	
	public String getLoadFileName() {
		return loadFileName;
	}


	public void setLoadFileName(String loadFileName) {
		this.loadFileName = loadFileName;
	}


	public static void main(String[] args) {
		new Base();
	}
}
