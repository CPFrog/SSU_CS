package events;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import layout.Base;

public class FileActionListener implements ActionListener {
	
	Base base;
	
	public FileActionListener(Base base){
		this.base = base;
	}
	
	
	public void actionPerformed(ActionEvent e) {
		String cmd = e.getActionCommand();
		switch (cmd) {
			//Toolbar ����
			case "New": //���θ����
				Utils.newBase(base);
				break;
			case "Open"://����
				Utils.open(base);
				break;
			case "Save"://��������
				Utils.save(base, base.getLoadFileName(), 1);
				break;
			case "Save as": //�ٸ��̸����� ����
				Utils.save(base, base.getLoadFileName(), 2);
				break;
			case "apply": //����
				Utils.apply(base);
				break;
			case "modify": //����
				Utils.modify(base);
				break;
			case "close":
				Closer c_t = new Closer();
				break;
			//Toolbar ��
			
			//�޴��� ����
			case "���� �����": //���θ����
				Utils.newBase(base);
				break;
			case "����"://����
				Utils.open(base);
				break;
			case "����"://��������
				Utils.save(base, base.getLoadFileName(), 1);
				break;
			case "�ٸ� �̸����� ����": //�ٸ��̸����� ����
				Utils.save(base, base.getLoadFileName(), 2);
				break;
			case "����": //����
				Utils.apply(base);
				break;
			case "����": //����
				Utils.modify(base);
				break;
			case "�ݱ�":
				Closer c_m = new Closer();
				break;
			//�޴��� ��	
		}
	}
}
