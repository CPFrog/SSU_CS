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
	
	//���ε�� �Է� �ؽ�Ʈ �ʵ�
	JTextArea text;
	
	//�̸� �ؽ�Ʈ�ʵ�
	JTextField textatb;
	
	//x��ǥ �ʵ�
	JTextField xTextField;
	
	//y��ǥ �ʵ�
	JTextField yTextField;
	
	//width �ʵ�
	JTextField wTextField;
	
	//height �ʵ�
	JTextField hTextField;
	
	//color�ʵ�
	JTextField colorTextField;
	
	Node clickNode;
	
	JPanel midp_a;
	
	public JPanel midp;
	
	
	String loadFileName = "";
	
	public Base() {
		
		this.base = this;
		
		setTitle("���ε��");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // â ������ �ڵ� ���α׷� ����
		Container c = getContentPane(); // ��ü ��
		Color bg_c = new Color(225,230,246);
		
		////////////// ���� �г� �׸� //////////////
		JPanel leftp = new JPanel(); // left panel
		leftp.setBackground(bg_c);
		text = new JTextArea("", 26, 16);// ���� �׸� �ؽ�Ʈ ���� �� ó�� ũ�� ����
		text.setTabSize(3); // ���ϸ� �ʹ� �ָ� ���� ���� 3 ���̸�ŭ���� ����
		JScrollPane lscroll = new JScrollPane(text); // text�� ScrollPane ���� 
		leftp.setLayout(new BorderLayout(0, 5)); // ���̾ƿ� ����, ���η� 5px ��ŭ ����
		leftp.add(lscroll, BorderLayout.CENTER); // �׸� �ؽ�Ʈ ������ ������ lscroll�� leftp�� ����
		JButton txapplybtn = new JButton("����"); // TeXt APPLY BuTtoN, �Է��� �׸� ���� ��ư ���� 
		txapplybtn.addMouseListener(new applyBtn());	
			
		
		leftp.add(txapplybtn, BorderLayout.SOUTH); // �׸� ���� ����ư�� ��ġ = �Ʒ�
		JLabel lefttitle = new JLabel("Text Editor Pane"); // ���� �г� �̸� ����
		leftp.add(lefttitle, BorderLayout.NORTH); // �г� �̸� ǥ�� ��ġ = ���
		
		////////////// ���� �г� �׸� ////////////// 
		JPanel rightp = new JPanel();// ������ ������ �� �� ����
		rightp.setBackground(bg_c);
		rightp.setLayout(new BorderLayout());// ������ �� ��ġ������ ����
		JLabel rightatLabel = new JLabel("Attribute Pane");
		rightp.add(rightatLabel, BorderLayout.NORTH);// �� ���� ����
		JPanel atpane = new JPanel();// �ؽ�Ʈ �Է� â �ø��� ���� �� ����
		atpane.setBackground(bg_c);
		rightp.add(atpane, BorderLayout.CENTER);// �� ��ġ ���� ����
		
		GridLayout grid = new GridLayout(6, 2, 2, 20);// �ؽ�Ʈ �Է� â �ø��� ���� ���� ��ġ������ ����. �׸���
		atpane.setLayout(grid);

		atpane.add(new JLabel("   TEXT: "));
		textatb = new JTextField(20); // â�� �� ���� 20
		textatb.setEditable(false); // �Է� ���޴� JTextField�� ����
		atpane.add(textatb); // ������ attribute pane�� ����
		atpane.add(new JLabel("   X: "));
		xTextField = new JTextField(20);
		atpane.add(xTextField); // â�� �� ���� 20
		
		atpane.add(new JLabel("   Y: "));
		yTextField = new JTextField(20);
		atpane.add(yTextField); // â�� �� ���� 20
		atpane.add(new JLabel("   W: "));
		wTextField = new JTextField(20);
		atpane.add(wTextField); // â�� �� ���� 20
		atpane.add(new JLabel("   H: "));
		hTextField = new JTextField(20);
		atpane.add(hTextField); // â�� �� ���� 20
		atpane.add(new JLabel("   Color: "));
		colorTextField = new JTextField(20);
		atpane.add(colorTextField); // â�� �� ���� 20
		
		JButton atapplybtn = new JButton("����");// ��ġ ���� �ʿ���
		
		atapplybtn.addMouseListener(new nodeChangeBtn());
		rightp.add(atapplybtn, BorderLayout.SOUTH);
		
		///////// �߾� �г� //////////
		midp = new JPanel();
		midp.setLayout(null);
		midp_a = new Midpane(this);
		midp.add(midp_a);
		

		JSplitPane pane1 = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, midp, rightp);
		pane1.setDividerLocation(750);//ù ���ø� ����
		JSplitPane pane2 = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, leftp, pane1);

		MenuBar menu = new MenuBar(this);
		ToolBar tool = new ToolBar(this);
		setJMenuBar(menu);
		c.add(tool, BorderLayout.NORTH);
		c.add(pane2);

		this.setSize(1024, 640);
		this.setVisible(true);

		// SplitPane ��ġ ����
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
