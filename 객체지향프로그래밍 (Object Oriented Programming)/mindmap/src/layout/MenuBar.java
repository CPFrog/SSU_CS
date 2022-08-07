package layout;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

import events.FileActionListener;

public class MenuBar extends JMenuBar {
	public MenuBar(Base base) {
		String[] fileTitle = { "���� �����", "����" }; // ����޴� �̸� ���
		JMenuItem[] fileItem = new JMenuItem[2]; // ����޴� ����
		JMenu filemenu = new JMenu("����");
		FileActionListener fileListener = new FileActionListener(base);
		for (int i = 0; i < fileTitle.length; i++) {
			fileItem[i] = new JMenuItem(fileTitle[i]); // �迭�θ� �����ߴ� �� ����޴� �׸� ���� MenuItem �Ӽ� �ο�
			fileItem[i].addActionListener(fileListener); // �� �׸񸶴� ������ ���
			filemenu.add(fileItem[i]);
		}

		String[] saveTitle = { "����", "�ٸ� �̸����� ����" };
		JMenuItem[] saveItem = new JMenuItem[2];
		JMenu savemenu = new JMenu("����");
		for (int i = 0; i < saveTitle.length; i++) {
			saveItem[i] = new JMenuItem(saveTitle[i]);
			saveItem[i].addActionListener(fileListener);
			savemenu.add(saveItem[i]);
		}

		String[] windowTitle = { "����", "����", "�ݱ�" };
		JMenuItem[] windowItem = new JMenuItem[3];
		JMenu windowmenu = new JMenu("â");
		for (int i = 0; i < windowTitle.length; i++) {
			windowItem[i] = new JMenuItem(windowTitle[i]);
			windowItem[i].addActionListener(fileListener); // �� �׸񸶴� ������ ���
			windowmenu.add(windowItem[i]);
		}

		this.add(filemenu);
		this.add(savemenu);
		this.add(windowmenu);
	}
}