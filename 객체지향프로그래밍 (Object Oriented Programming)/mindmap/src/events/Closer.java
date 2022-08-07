package events;

//import java.awt.event.ActionEvent;
import javax.swing.JOptionPane;

public class Closer {
	public Closer() {
		int result = JOptionPane.showConfirmDialog(null, "저장하지 않은 데이터는 소멸됩니다. \n 정말 종료하시겠습니까?", "Close", JOptionPane.YES_NO_OPTION);
		if(result == JOptionPane.CLOSED_OPTION) {
			// 사용자가 "예"나 "아니오"의 선택없이 다이얼로그창을닫은 경우
			return; // not exited
		}
		else if(result == JOptionPane.YES_OPTION) {
			// 사용자가 "예"를 선택한 경우
			System.exit(0);
		}
		else {	// 사용자가 "아니오"를 선택한 경우
			return; // not exited
		}
	}
}