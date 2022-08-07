package layout;
import javax.swing.JToolBar;
import javax.swing.ToolTipManager;

import events.FileActionListener;

import javax.swing.JButton;

public class ToolBar extends JToolBar {	
	public ToolBar(Base base){
		FileActionListener fileListener = new FileActionListener(base); 
		
		JButton newBtn = new JButton("New");
		newBtn.setToolTipText("�� ���ε�� ����"); //���� ����
		newBtn.addActionListener(fileListener);
		this.add(newBtn);
		this.addSeparator();
		
		JButton openBtn = new JButton("Open");
		openBtn.setToolTipText("���ε�� �ҷ�����"); // ����
		openBtn.addActionListener(fileListener);
		this.add(openBtn);		
		this.addSeparator();
		
		JButton saveBtn = new JButton("Save");
		saveBtn.setToolTipText("���ε�� ����"); //����
		saveBtn.addActionListener(fileListener);
		this.add(saveBtn);
		this.addSeparator();
		
		JButton saveasBtn = new JButton("Save as");
		saveasBtn.setToolTipText("�ٸ� �̸����� ����"); //����
		saveasBtn.addActionListener(fileListener);
		this.add(saveasBtn);
		this.addSeparator();
		
		JButton closeBtn = new JButton("Close");
		closeBtn.setToolTipText("���ε�� �ݱ�"); //����
		closeBtn.addActionListener(fileListener);
		this.add(closeBtn);
		this.addSeparator();
		
		JButton applyBtn = new JButton("apply");
		applyBtn.setToolTipText("���ε�ʿ� ����"); //����
		applyBtn.addActionListener(fileListener);
		this.add(applyBtn);
		this.addSeparator();
		
		JButton modBtn = new JButton("modify");
		modBtn.setToolTipText("�Ӽ� ����"); //����
		modBtn.addActionListener(fileListener);
		this.add(modBtn);
		this.addSeparator();
		
		//���� ������,���ӽð� ����
		ToolTipManager m = ToolTipManager.sharedInstance();
		m.setInitialDelay(500); // �߱���� ������ 0.5��
		m.setDismissDelay(4000); // ���� ���ӽð� 4��
		
		this.setFloatable(false);
	}
}