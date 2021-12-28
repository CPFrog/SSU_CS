import java.util.HashMap;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;

/**
 * 모든 instruction의 정보를 관리하는 클래스. instruction data들을 저장한다 또한 instruction 관련 연산,
 * 예를 들면 목록을 구축하는 함수, 관련 정보를 제공하는 함수 등을 제공 한다.
 */
public class InstTable {
	/**
	 * inst.data 파일을 불러와 저장하는 공간. 명령어의 이름을 집어넣으면 해당하는 Instruction의 정보들을 리턴할 수 있다.
	 */
	HashMap<String, Instruction> instMap;

	public static final int MAX_INST = 256;

	/**
	 * 클래스 초기화. 파싱을 동시에 처리한다.
	 * 
	 * @param instFile : instruction에 대한 명세가 저장된 파일 이름
	 */

	public InstTable(String instFile) {
		instMap = new HashMap<String, Instruction>();
		openFile(instFile);
	}

	/**
	 * 입력받은 이름의 파일을 열고 해당 내용을 파싱하여 instMap에 저장한다.
	 */
	public void openFile(String fileName) {
		try {
			File file = new File(fileName);
			FileReader filereader = new FileReader(file);
			BufferedReader bufReader = new BufferedReader(filereader);
			String line = "";
			while ((line = bufReader.readLine()) != null) {
				Instruction inst = new Instruction(line);
				instMap.put(inst.instruction, inst);
			}
			bufReader.close();
		} catch (FileNotFoundException e) {
			System.out.println(e);
		} catch (IOException e) {
			System.out.println(e);
		}
	}

	/**
	 * 입력받은 이름의 명령어에 대한 정보를 반환
	 * 
	 * @param name : 검색하려고 하는 명령어의 이름
	 * @return : 명령어에 대한 정보가 담긴 Instruction
	 */
	private Instruction search(String name) {
		return instMap.get(nameToKey(name));
	}

	/**
	 * 입력받은 이름의 명령어가 instTable에 있는지 판단해주는 함수.
	 * 
	 * @param name : instTable에 있는지 판단하려는 명령어
	 * @return : 명령어 테이블에 있는경우 true, 없는경우 false.
	 */
	public boolean exists(String name) {
		return instMap.containsKey(nameToKey(name));
	}

	/**
	 * 입력받은 operator가 어떤 형태이던지 간에 instTable의 검색에 사용 가능한 형태로 바꿔주는 함수.
	 * 
	 * @param name : 변환되기 전 String 형태의 operator
	 * @return : instTable에서 검색 가능한 형태로 변환된 String 형태의 operator.
	 */
	private String nameToKey(String name) {
		return name.replace("+", "");
	}

	/**
	 * 입력받은 이름을 가진 명령어가 몇 형식인지 찾아주는 함수.
	 * 
	 * @param name : 찾으려는 명령어의 이름
	 * @return : 명령어의 format
	 * @exception : 입력받은 이름을 가진 명령어가 없을 경우 Exception throw.
	 */
	public int getFormat(String name) throws Exception {
		if (name.charAt(0) == '+')
			return 4;
		if (!exists(name))
			throw new Exception("없는 명령어입니다.");
		return search(name).format;
	}

	/**
	 * 입력받은 이름을 가진 명령어가 있다면 그 명령어의 opcode를, 아닐 경우 Exception을 throw한다.
	 * 
	 * @param name : 찾으려는 명령어의 이름
	 * @return : 명령어의 opcode
	 * @exception : 입력받은 이름을 가진 명령어가 없다면 오류문과 함께 Exception이 Throw됨.
	 */
	public int getOpCode(String name) throws Exception {
		if (!exists(name))
			throw new Exception("없는 명령어입니다.");
		return search(name).opcode;
	}

	public Instruction getName(int opcode) {
		for (String o : instMap.keySet()) {
			if (instMap.get(o).opcode == opcode) {
				return instMap.get(o);
			}
		}
		return null;
	}
}

/**
 * 명령어 하나하나의 구체적인 정보는 Instruction클래스에 담긴다. instruction과 관련된 정보를 저장하고 기초적인 연산을
 * 수행한다.
 */
class Instruction {

	String instruction;
	int opcode;
	int numberOfOperand;

	/** instruction이 몇 바이트 명령어인지 저장. 이후 편의성을 위함 */
	int format;

	/**
	 * 클래스를 선언하면서 일반문자열을 즉시 구조에 맞게 파싱한다.
	 * 
	 * @param line : instruction 명세파일로부터 한줄씩 가져온 문자열
	 */
	public Instruction(String line) {
		parsing(line);
	}

	/**
	 * 일반 문자열을 파싱하여 instruction 정보를 파악하고 저장한다.
	 * 
	 * @param line : instruction 명세파일로부터 한줄씩 가져온 문자열
	 */
	public void parsing(String line) {
		String[] line_seperate = line.split("\t");
		instruction = line_seperate[0];
		format = Integer.parseInt(line_seperate[1]);
		opcode = Integer.parseInt(line_seperate[2], 16);
		numberOfOperand = Integer.parseInt(line_seperate[3]);
	}

}
