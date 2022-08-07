package layout;
import javax.swing.JToolBar;
import javax.swing.ToolTipManager;

import events.FileActionListener;

import javax.swing.JButton;

public class ToolBar extends JToolBar {	
	public ToolBar(Base base){
		FileActionListener fileListener = new FileActionListener(base); 
		
		JButton newBtn = new JButton("New");
		newBtn.setToolTipText("새 마인드맵 생성"); //툴팁 설정
		newBtn.addActionListener(fileListener);
		this.add(newBtn);
		this.addSeparator();
		
		JButton openBtn = new JButton("Open");
		openBtn.setToolTipText("마인드맵 불러오기"); // 툴팁
		openBtn.addActionListener(fileListener);
		this.add(openBtn);		
		this.addSeparator();
		
		JButton saveBtn = new JButton("Save");
		saveBtn.setToolTipText("마인드맵 저장"); //툴팁
		saveBtn.addActionListener(fileListener);
		this.add(saveBtn);
		this.addSeparator();
		
		JButton saveasBtn = new JButton("Save as");
		saveasBtn.setToolTipText("다른 이름으로 저장"); //툴팁
		saveasBtn.addActionListener(fileListener);
		this.add(saveasBtn);
		this.addSeparator();
		
		JButton closeBtn = new JButton("Close");
		closeBtn.setToolTipText("마인드맵 닫기"); //툴팁
		closeBtn.addActionListener(fileListener);
		this.add(closeBtn);
		this.addSeparator();
		
		JButton applyBtn = new JButton("apply");
		applyBtn.setToolTipText("마인드맵에 적용"); //툴팁
		applyBtn.addActionListener(fileListener);
		this.add(applyBtn);
		this.addSeparator();
		
		JButton modBtn = new JButton("modify");
		modBtn.setToolTipText("속성 변경"); //툴팁
		modBtn.addActionListener(fileListener);
		this.add(modBtn);
		this.addSeparator();
		
		//툴팁 딜레이,지속시간 설정
		ToolTipManager m = ToolTipManager.sharedInstance();
		m.setInitialDelay(500); // 뜨기까지 딜레이 0.5초
		m.setDismissDelay(4000); // 툴팁 지속시간 4초
		
		this.setFloatable(false);
	}
}