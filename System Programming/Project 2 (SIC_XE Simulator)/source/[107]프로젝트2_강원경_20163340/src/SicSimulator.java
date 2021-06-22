import java.io.File;

/**
 * 시뮬레이터로서의 작업을 담당한다. VisualSimulator에서 사용자의 요청을 받으면 이에 따라 ResourceManager에 접근하여
 * 작업을 수행한다.
 * 
 * 작성중의 유의사항 : 1) 새로운 클래스, 새로운 변수, 새로운 함수 선언은 얼마든지 허용됨. 단, 기존의 변수와 함수들을 삭제하거나
 * 완전히 대체하는 것은 지양할 것. 2) 필요에 따라 예외처리, 인터페이스 또는 상속 사용 또한 허용됨. 3) 모든 void 타입의 리턴값은
 * 유저의 필요에 따라 다른 리턴 타입으로 변경 가능. 4) 파일, 또는 콘솔창에 한글을 출력시키지 말 것. (채점상의 이유. 주석에 포함된
 * 한글은 상관 없음)
 * 
 * + 제공하는 프로그램 구조의 개선방법을 제안하고 싶은 분들은 보고서의 결론 뒷부분에 첨부 바랍니다. 내용에 따라 가산점이 있을 수
 * 있습니다.
 */
public class SicSimulator {
	ResourceManager rMgr;
	private SicLoader loader;
	InstTable instTab;
	InstLuncher instLauncher;
	private int currentAddr;
	int returnAddr;
	boolean isStart;
	boolean hasEnd;
	VisualSimulator vs;

	private final int END_ADDR = 0x1003;

	public SicSimulator(ResourceManager resourceManager) {
		// 필요하다면 초기화 과정 추가
		this.rMgr = resourceManager;
		isStart = true;
		hasEnd = false;
		loader = new SicLoader(this.rMgr);
		instTab = new InstTable("inst.data");
		instLauncher = new InstLuncher(resourceManager);
	}

	/**
	 * 레지스터, 메모리 초기화 등 프로그램 load와 관련된 작업 수행. 단, object code의 메모리 적재 및 해석은
	 * SicLoader에서 수행하도록 한다.
	 */
	public void load(File program) {
		/* 메모리 초기화, 레지스터 초기화 등 */
		rMgr.initializeResource();

	}

	public void load(File program, SicLoader sicloader, VisualSimulator _vs) {
		loader = sicloader;
		vs = _vs;
		load(program);
	}

	/**
	 * 1개의 instruction이 수행된 모습을 보인다.
	 */
	public void oneStep() {
		vs.update();
		char[] temp;
		loader.setSection(currentAddr);
		if (isStart) {
			rMgr.firstInst = currentAddr;
			rMgr.opStartAddr = currentAddr;
			isStart = false;
		}
		temp = rMgr.getMemory(currentAddr, 2);
		rMgr.targetAddr = 0;

		int op = (temp[0]) & 0xFC;
		int pc = 0;
		Instruction inst = instTab.getName(op);
		if (inst != null) {
			addLog(inst.instruction);
		}
		int format;
		if (inst.format == 2) {
			format = 2;
		} else {
			if ((temp[1] & 0x10) == 0)
				format = 3;
			else
				format = 4;
		}
		
		// 명령어가 2형식일 때 
		if (format == 2) {
			pc = currentAddr + 2;
			temp = rMgr.getMemory(currentAddr, 2);
			rMgr.instList.add(beforeLogged(temp));

			int reg = temp[1];
			int reg1 = reg >> 4;
			int reg2 = reg & 0x0f;

			switch (inst.instruction) {
			case "CLEAR":
				instLauncher.clear(reg1);
				break;
			case "COMPR":
				instLauncher.compr(reg1, reg2);
				break;
			case "TIXR":
				instLauncher.tixr(reg1);
				break;
			}

			currentAddr = pc;
		} else if (format == 3) { //명령어가 3형식일 때
			pc = currentAddr + 3;
			temp = rMgr.getMemory(currentAddr, 3);
			rMgr.instList.add(beforeLogged(temp));

			int disp = ((((int) temp[1]) & 0x0f) << 8) | (int) temp[2];
			int ta = pc + disp;
			if (ta > 4218) {
				ta -= 4096;
			}
			rMgr.targetAddr = ta;
			currentAddr += 3;

			switch (inst.instruction) {
			case "LDT":
				instLauncher.ldt(disp, pc, 3);
				break;
			case "TD":
				instLauncher.td(disp, pc);
				break;
			case "RD":
				instLauncher.rd();
				break;
			case "STA":
				instLauncher.sta(disp, pc);
				break;
			case "WD":
				instLauncher.wd();
				break;
			case "JEQ":
				currentAddr = instLauncher.jeq(disp, pc, currentAddr);
				break;
			case "COMP":
				instLauncher.comp(disp);
				break;
			case "JLT":
				currentAddr = instLauncher.jlt(disp, pc, currentAddr);
				break;
			case "RSUB":
				currentAddr = instLauncher.rsub(returnAddr);
				break;
			case "LDA":
				instLauncher.lda(temp, disp, pc, 3);
				break;
			case "J":
				currentAddr = instLauncher.j(disp, pc);
				hasEnd = (currentAddr == END_ADDR);
				break;
			}
		} else { //명령어가 4형식일 때
			pc = currentAddr + 4;
			temp = rMgr.getMemory(currentAddr, 4);
			int addr = (temp[1] & 0x0f) << 16;
			addr |= temp[2] << 8;
			addr |= temp[3];
			rMgr.targetAddr = addr;
			rMgr.instList.add(beforeLogged(temp));
			switch (inst.instruction) {
			case "JSUB":
				// 이 함수는 Simulator 내에서 처리하는게 훨씬 효율적이라 이렇게 처리함.
				isStart = true;
				returnAddr = pc;
				currentAddr = addr;
				break;
			case "STCH":
				instLauncher.stch(addr);
				currentAddr = pc;
				break;
			case "STX":
				instLauncher.stx(addr);
				currentAddr = pc;
				break;
			case "LDA":
				instLauncher.lda(temp, addr, pc, 4);
				break;
			case "LDT":
				instLauncher.ldt(addr, 0, 4);
				currentAddr = pc;
				break;
			case "LDCH":
				instLauncher.ldch(addr);
				currentAddr = pc;
				break;
			}
		}

	}

	/**
	 * 남은 모든 instruction이 수행된 모습을 보인다.
	 */
	public void allStep() {
		while (!hasEnd) {
			oneStep();
		}
		rMgr.closeDevice();
	}

	/**
	 * 각 단계를 수행할 때 마다 관련된 기록을 남기도록 한다.
	 */
	public void addLog(String log) {
		rMgr.logText += log + "\n";
	}

	private String beforeLogged(char[] temp) {
		int tempaddr = 0;
		for (char c : temp) {
			tempaddr <<= 8;
			tempaddr |= c;
		}
		return Integer.toHexString(tempaddr).toUpperCase();
	}
}
