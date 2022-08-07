package layout;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

import events.FileActionListener;

public class MenuBar extends JMenuBar {
	public MenuBar(Base base) {
		String[] fileTitle = { "새로 만들기", "열기" }; // 서브메뉴 이름 목록
		JMenuItem[] fileItem = new JMenuItem[2]; // 서브메뉴 갯수
		JMenu filemenu = new JMenu("파일");
		FileActionListener fileListener = new FileActionListener(base);
		for (int i = 0; i < fileTitle.length; i++) {
			fileItem[i] = new JMenuItem(fileTitle[i]); // 배열로만 구성했던 각 서브메뉴 항목에 실제 MenuItem 속성 부여
			fileItem[i].addActionListener(fileListener); // 각 항목마다 리스너 등록
			filemenu.add(fileItem[i]);
		}

		String[] saveTitle = { "저장", "다른 이름으로 저장" };
		JMenuItem[] saveItem = new JMenuItem[2];
		JMenu savemenu = new JMenu("저장");
		for (int i = 0; i < saveTitle.length; i++) {
			saveItem[i] = new JMenuItem(saveTitle[i]);
			saveItem[i].addActionListener(fileListener);
			savemenu.add(saveItem[i]);
		}

		String[] windowTitle = { "적용", "변경", "닫기" };
		JMenuItem[] windowItem = new JMenuItem[3];
		JMenu windowmenu = new JMenu("창");
		for (int i = 0; i < windowTitle.length; i++) {
			windowItem[i] = new JMenuItem(windowTitle[i]);
			windowItem[i].addActionListener(fileListener); // 각 항목마다 리스너 등록
			windowmenu.add(windowItem[i]);
		}

		this.add(filemenu);
		this.add(savemenu);
		this.add(windowmenu);
	}
}