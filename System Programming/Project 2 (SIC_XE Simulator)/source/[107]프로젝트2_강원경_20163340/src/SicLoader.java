import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.StringTokenizer;

/**
 * SicLoader는 프로그램을 해석해서 메모리에 올리는 역할을 수행한다. 이 과정에서 linker의 역할 또한 수행한다.
 * 
 * SicLoader가 수행하는 일을 예를 들면 다음과 같다. - program code를 메모리에 적재시키기 - 주어진 공간만큼 메모리에 빈
 * 공간 할당하기 - 과정에서 발생하는 symbol, 프로그램 시작주소, control section 등 실행을 위한 정보 생성 및 관리
 */
public class SicLoader {
	ResourceManager rMgr;
	private int programAddr;
	private int csAddr;
	private int execAddr;
	ArrayList<Integer> length;
	ArrayList<String> csName;
	ArrayList<String> endLineAddr;

	public SicLoader(ResourceManager resourceManager) {
		setResourceManager(resourceManager);
	}

	/**
	 * Loader와 프로그램을 적재할 메모리를 연결시킨다.
	 * 
	 * @param resourceManager 리소스 매니저
	 */
	public void setResourceManager(ResourceManager resourceManager) {
		this.rMgr = resourceManager;
	}

	/**
	 * object code를 읽어서 load과정을 수행한다. load한 데이터는 resourceManager가 관리하는 메모리에 올라가도록
	 * 한다. load과정에서 만들어진 symbol table 등 자료구조 역시 resourceManager에 전달한다.
	 * 
	 * @param objectCode 읽어들인 파일
	 */
	public void load(File objectCode) {
		length = new ArrayList<Integer>();
		csName = new ArrayList<String>();
		endLineAddr = new ArrayList<String>();
		pass1(objectCode);
		pass2(objectCode);
		// object code 해석 -> resourceManager setMemory 해주고, symbolTable set해주면됨

	};

	public void setSection(int currentAddr) {
		int section = 0;
		int len = 0;
		for (section = 0; section < length.size(); section++) {
			len += length.get(section);
			if (currentAddr < len)
				break;
		}
		rMgr.programName = csName.get(section);
		rMgr.programLength = length.get(section);
	}

	// 각 Control Section별로 프로그램의 이름, 길이를 저장하고 D레코드의 Symbol들을 SymbolTable에 저장한다. 
	private void pass1(File objectCode) {
		SymbolTable symbolTable = rMgr.symtabList;
		csAddr = programAddr;
		try {
			Scanner s = new Scanner(objectCode);

			while (s.hasNext()) {

				String line = s.nextLine();
				if (line.equals("\n") || line.equals(""))
					continue;
				String[] headLine = line.split("\t");
				int controlsection_Length = Integer.parseInt(headLine[1].substring(6), 16);
				length.add(controlsection_Length);
				String sectionName = headLine[0].substring(1);
				csName.add(sectionName);
				symbolTable.symbolList.add(sectionName);
				symbolTable.addressList.add(csAddr);

				line = s.nextLine();
				while (line.charAt(0) != 'E') { // 하나의 Control Section이 끝날 때까지
					if (line.charAt(0) == 'D') { // 다른 테이블에서 참조하게 될 symbol일 때
						StringTokenizer strtok = new StringTokenizer(line.substring(1), "0123456789");
						String[] extdef = new String[strtok.countTokens() * 2];
						int j = 0;
						while (strtok.hasMoreTokens()) {
							extdef[j] = strtok.nextToken();
							j += 2;
						}
						j = 0;

						int offset = 1;
						for (int i = 0; i < extdef.length - 2; i += 2) {
							offset += extdef[i].length();
							extdef[i + 1] = line.substring(offset, offset + 6);
							offset += 6;
						}

						for (int i = 1; i < extdef.length - 2; i += 2) {
							String symName = extdef[i - 1];
							int symAddr = Integer.parseInt(extdef[i], 16);
							symbolTable.symbolList.add(symName);
							symbolTable.addressList.add(symAddr + csAddr);
						}
					}
					line = s.nextLine();
				}
				endLineAddr.add(line.substring(1));
				csAddr += controlsection_Length;
			}
			s.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}

	// T레코드인 경우 리소스 매니저에 ObjectCode를 한 줄씩 메모리 영역에 올려준다.
	// M레코드인 경우 해당 Symbol의 실제주소값을 저장하여 저장한다.
	private void pass2(File objectCode) {
		csAddr = programAddr;
		execAddr = programAddr;
		try {
			Scanner s = new Scanner(objectCode);

			while (s.hasNext()) {
				String line = s.nextLine();
				if (line.equals("\n") || line.equals(""))
					continue;
				String[] headLine = line.split("\t");
				int controlsection_Length = Integer.parseInt(headLine[1].substring(6), 16);
				line = s.nextLine();
				execAddr = csAddr;
				while (line.charAt(0) != 'E') {
					if (line.charAt(0) == 'T') {
						char[] temp = new char[60];
						int templen = 0;
						String textLine = line.substring(9);
						for (int i = 0; i < textLine.length(); i += 2) {
							String strByte = textLine.substring(i, i + 2);
							temp[templen++] = (char) Integer.parseInt(strByte, 16);
						}
						rMgr.setMemory(execAddr, temp, templen);
						execAddr += Integer.parseInt(line.substring(7, 9), 16);
					}

					else if (line.charAt(0) == 'M') { // M레코드일 때 : 레코드의 값 변경 필요.
						int addr = -1;
						String symName = line.substring(10);

						addr = rMgr.symtabList.search(symName);

						char[] modByte = new char[3];
						for (int i = 2; i >= 0; i--) {
							modByte[i] = (char) ((addr >> (8 * Math.abs(i - 2))) & 0xFF);
						}
						if (line.charAt(9) == '+') {
							int modAddr = Integer.parseInt(line.substring(1, 7), 16);
							char[] changed = rMgr.getMemory(csAddr + modAddr, 3);
							for (int i = 0; i < 3; i++)
								changed[i] += modByte[i];

							rMgr.setMemory(csAddr + modAddr, changed, 3);
						} else if (line.charAt(9) == '-') {
							int modAddr = Integer.parseInt(line.substring(1, 7), 16);
							char[] changed = rMgr.getMemory(csAddr + modAddr, 3);
							for (int i = 0; i < 3; i++)
								changed[i] -= modByte[i];

							rMgr.setMemory(csAddr + modAddr, changed, 3);
						}

					}
					line = s.nextLine();
				}
				csAddr += controlsection_Length;
			}
			s.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}

	void setStartAddr(int startAddr) {
		this.programAddr = startAddr;
	}

	int getStartAddr() {
		return programAddr;
	}
}
