import javax.swing.*;
import java.io.File;

/**
 * VisualSimulator는 사용자와의 상호작용을 담당한다. 즉, 버튼 클릭등의 이벤트를 전달하고 그에 따른 결과값을 화면에 업데이트
 * 하는 역할을 수행한다.
 *
 * 실제적인 작업은 SicSimulator에서 수행하도록 구현한다.
 */
public class VisualSimulator {
	ResourceManager resourceManager = new ResourceManager();
	SicLoader sicLoader = new SicLoader(resourceManager);
	SicSimulator sicSimulator = new SicSimulator(resourceManager);
	UserInterface frame = new UserInterface(this);

	/**
	 * 프로그램 로드 명령을 전달한다.
	 */
	public void load(File program) {
		sicLoader.load(program);
		sicSimulator.load(program, sicLoader, this);
		update();
	};

	/**
	 * 하나의 명령어만 수행할 것을 SicSimulator에 요청한다.
	 */
	public void oneStep() {
		sicSimulator.oneStep();
		update();
	};

	/**
	 * 남아있는 모든 명령어를 수행할 것을 SicSimulator에 요청한다.
	 */
	public void allStep() {
		sicSimulator.allStep();
		update();
	};

	/**
	 * 화면을 최신값으로 갱신하는 역할을 수행한다.
	 */
	public void update() {
		
		frame.deviceTF.setText(resourceManager.curDevice); // 사용중인 장치
		frame.programNameTF.setText(resourceManager.programName); // 프로그램 이름
		frame.lengthTF.setText(Integer.toHexString(resourceManager.programLength).toUpperCase()); // 프로그램 길이
		frame.firstInstTF.setText(Integer.toHexString(resourceManager.firstInst).toUpperCase()); // 시작주소
		frame.opStartAddrTF.setText(Integer.toHexString(resourceManager.opStartAddr).toUpperCase()); 
		frame.taTF.setText(Integer.toHexString(resourceManager.targetAddr)); // TargetAddress
		frame.instList.setListData(resourceManager.instList); // Instructions
		frame.LogTA.setText(resourceManager.logText); // 프로그램 하단 로그
		frame.memStartAddrTF.setText(Integer.toHexString(resourceManager.memStartAddr).toUpperCase()); // 시작 주소

		//화면에 보이는 레지스터 값 갱신
		for (int i = 0; i < resourceManager.REGISTER_NUMBER; i++) {
			if (i == 0) {
				frame.decATF.setText(Integer.toString(resourceManager.getRegister(i)));
				frame.hexATF.setText(Integer.toHexString(resourceManager.getRegister(i)));
			} else if (i == 1) {
				frame.decXTF.setText(Integer.toString(resourceManager.getRegister(i)));
				frame.hexXTF.setText(Integer.toHexString(resourceManager.getRegister(i)));
			} else if (i == 2) {
				frame.decLTF.setText(Integer.toString(resourceManager.getRegister(i)));
				frame.hexLTF.setText(Integer.toHexString(resourceManager.getRegister(i)));
			} else if (i == 3) {
				frame.decBTF.setText(Integer.toString(resourceManager.getRegister(i)));
				frame.hexBTF.setText(Integer.toHexString(resourceManager.getRegister(i)));
			} else if (i == 4) {
				frame.decSTF.setText(Integer.toString(resourceManager.getRegister(i)));
				frame.hexSTF.setText(Integer.toHexString(resourceManager.getRegister(i)));
			} else if (i == 5) {
				frame.decTTF.setText(Integer.toString(resourceManager.getRegister(i)));
				frame.hexTTF.setText(Integer.toHexString(resourceManager.getRegister(i)));
			} else if (i == 6) {
				frame.hexFTF.setText(Integer.toHexString(resourceManager.getRegister(i)));
			} else if (i == 8) {
				frame.decPCTF.setText(Integer.toString(resourceManager.getRegister(i)));
				frame.hexPCTF.setText(Integer.toHexString(resourceManager.getRegister(i)));
			} else if (i == 9) {
				frame.hexSWTF.setText(Integer.toHexString(resourceManager.getRegister(i)));
			}
		}
	}

	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				VisualSimulator visualSimulator = new VisualSimulator();
				visualSimulator.frame.setVisible(true);
			}
		});
	}
}
