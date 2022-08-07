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
			//Toolbar 시작
			case "New": //새로만들기
				Utils.newBase(base);
				break;
			case "Open"://열기
				Utils.open(base);
				break;
			case "Save"://파일저장
				Utils.save(base, base.getLoadFileName(), 1);
				break;
			case "Save as": //다른이름으로 저장
				Utils.save(base, base.getLoadFileName(), 2);
				break;
			case "apply": //적용
				Utils.apply(base);
				break;
			case "modify": //변경
				Utils.modify(base);
				break;
			case "close":
				Closer c_t = new Closer();
				break;
			//Toolbar 끝
			
			//메뉴바 시작
			case "새로 만들기": //새로만들기
				Utils.newBase(base);
				break;
			case "열기"://열기
				Utils.open(base);
				break;
			case "저장"://파일저장
				Utils.save(base, base.getLoadFileName(), 1);
				break;
			case "다른 이름으로 저장": //다른이름으로 저장
				Utils.save(base, base.getLoadFileName(), 2);
				break;
			case "적용": //적용
				Utils.apply(base);
				break;
			case "변경": //변경
				Utils.modify(base);
				break;
			case "닫기":
				Closer c_m = new Closer();
				break;
			//메뉴바 끝	
		}
	}
}
