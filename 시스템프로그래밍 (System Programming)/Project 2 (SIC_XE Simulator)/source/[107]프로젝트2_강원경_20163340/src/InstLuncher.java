// instruction에 따라 동작을 수행하는 메소드를 정의하는 클래스

public class InstLuncher {
	ResourceManager rMgr;
	char[] Data;

	public InstLuncher(ResourceManager resourceManager) {
		this.rMgr = resourceManager;
	}

	// instruction 별로 동작을 수행하는 메소드를 정의
	public void clear(int reg) {
		rMgr.setRegister(reg, 0);
	}

	public void compr(int reg1, int reg2) {
		if (rMgr.getRegister(reg1) - rMgr.MAX_MEMORY_SIZE + 1 == rMgr.getRegister(reg2))
			rMgr.setRegister(9, 1);
	}

	public void tixr(int reg) {
		int x = rMgr.getRegister(1);
		rMgr.setRegister(1, rMgr.getRegister(1) + 1);
		if (x < rMgr.getRegister(reg))
			rMgr.setRegister(9, 2);
		else
			rMgr.setRegister(9, 0);
	}

	public void ldt(int disp, int pc, int format) {
		if (format == 3) {
			char[] targetAddress = rMgr.getMemory(disp + pc, 3);
			int value = (targetAddress[0] << 16);
			value |= targetAddress[1] << 8;
			value |= targetAddress[2];
			rMgr.setRegister(5, value);
		} else {
			char[] targetAddress = rMgr.getMemory(disp, 3);
			int value = (targetAddress[0] << 16);
			value |= (targetAddress[1] << 8);
			value |= (targetAddress[2]);

			rMgr.setRegister(5, value - 1);
		}
	}

	public void td(int disp, int pc) {
		char[] dev = rMgr.getMemory(pc + disp, 2);
		rMgr.testDevice(String.format("%02X", (int) dev[0]));
		rMgr.setRegister(9, 0);
	}

	public void rd() {
		Data = rMgr.readDevice(rMgr.curDevice, 1);
		rMgr.setRegister(0, Data[0]);
	}

	public void sta(int disp, int pc) {
		int x = rMgr.getRegister(0);
		char[] regA = new char[3];
		regA[0] = (char) (x >> 16);
		regA[1] = (char) ((x >> 8) & 0xff);
		regA[2] = (char) x;

		rMgr.setMemory(disp + pc, regA, 3);
	}

	public void wd() {
		int a = rMgr.getRegister(0);
		char[] va = new char[1];
		va[0] = (char) a;
		rMgr.writeDevice(rMgr.curDevice, va, 1);
	}

	public int jeq(int disp, int pc, int currentAddr) {
		int sw = rMgr.getRegister(9);
		if (sw == 1) {
			currentAddr = pc + disp;
		}
		return currentAddr;
	}

	public void comp(int disp) {
		int a = rMgr.getRegister(0);
		if (a == disp)
			rMgr.setRegister(9, 1);
		else if (a < disp)
			rMgr.setRegister(9, 2);
		else
			rMgr.setRegister(9, 3);
	}

	public int jlt(int disp, int pc, int currentAddr) {
		int sw = rMgr.getRegister(9);
		if (sw == 2) {
			currentAddr = disp + pc;
			if (currentAddr > 4218) {
				currentAddr -= 4096;
			}
		}
		return currentAddr;
	}

	public int rsub(int retAddr) {
		rMgr.curDevice = "";
		return retAddr;
	}

	public void lda(char[] regVal, int disp, int pc, int format) {
		if (regVal[0] == 0x01)
			rMgr.setRegister(0, disp);
		else {
			char[] taValue = new char[3];
			if (format == 3)
				taValue = rMgr.getMemory(disp + pc, 3);
			else
				taValue = rMgr.getMemory(disp, 3);

			int value = taValue[0] << 16;
			value |= taValue[1] << 8;
			value |= taValue[2];
			rMgr.setRegister(0, value);
		}
	}

	public int j(int disp, int pc) {
		return disp + pc;
	}

	public void stch(int addr) {
		int a = rMgr.getRegister(0);
		char[] cA = new char[1];
		cA[0] = (char) a;
		rMgr.setMemory(addr + rMgr.getRegister(1), cA, 1);
	}

	public void stx(int addr) {
		int x = rMgr.getRegister(1);
		char[] cX = new char[3];
		cX[0] = (char) (x >> 16);
		cX[1] = (char) ((x >> 8) & 0xff);
		cX[2] = (char) x;
		rMgr.setMemory(addr, cX, 3);
	}

	public void ldch(int addr) {
		char[] value = rMgr.getMemory(addr + rMgr.getRegister(1), 1);
		rMgr.setRegister(0, value[0]);
	}
}