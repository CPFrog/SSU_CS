package events;

//import java.awt.event.ActionEvent;
import javax.swing.JOptionPane;

public class Closer {
	public Closer() {
		int result = JOptionPane.showConfirmDialog(null, "�������� ���� �����ʹ� �Ҹ�˴ϴ�. \n ���� �����Ͻðڽ��ϱ�?", "Close", JOptionPane.YES_NO_OPTION);
		if(result == JOptionPane.CLOSED_OPTION) {
			// ����ڰ� "��"�� "�ƴϿ�"�� ���þ��� ���̾�α�â������ ���
			return; // not exited
		}
		else if(result == JOptionPane.YES_OPTION) {
			// ����ڰ� "��"�� ������ ���
			System.exit(0);
		}
		else {	// ����ڰ� "�ƴϿ�"�� ������ ���
			return; // not exited
		}
	}
}