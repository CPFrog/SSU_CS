import java.util.ArrayList;

/**
 * 사용자가 작성한 프로그램 코드를 단어별로 분할 한 후, 의미를 분석하고, 최종 코드로 변환하는 과정을 총괄하는 클래스이다.
 * 
 * pass2에서 object code로 변환하는 과정은 혼자 해결할 수 없고 symbolTable과 instTable의 정보가 필요하므로
 * 이를 링크시킨다. section 마다 인스턴스가 하나씩 할당된다.
 *
 */

public class TokenTable {
	public static final int MAX_OPERAND = 3;

	/* bit 조작의 가독성을 위한 선언 */
	public static final int nFlag = 32;
	public static final int iFlag = 16;
	public static final int xFlag = 8;
	public static final int bFlag = 4;
	public static final int pFlag = 2;
	public static final int eFlag = 1;

	/* Token을 다룰 때 필요한 테이블들을 링크시킨다. */
	LabelTable symTab;
	LabelTable literalTab;
	InstTable instTab;

	/** 각 line을 의미별로 분할하고 분석하는 공간. */
	ArrayList<Token> tokenList = new ArrayList<Token>();
	
	/** 현재 Control Section의 location counter.
	 * 해당 Control Section의 parsing이 끝나면 최종적으로
	 * 그 섹션의 프로그램 길이가 저장된다. */
	int locctr = 0; 

	/**
	 * 초기화하면서 symTable과 instTable을 링크시킨다.
	 * 
	 * @param symTab    : 해당 section과 연결되어있는 symbol table
	 * @param literaTab : 해당 section과 연결되어있는 literal table
	 * @param instTab   : instruction 명세가 정의된 instTable
	 */
	public TokenTable(LabelTable symTab, LabelTable literalTab, InstTable instTab) {
		this.symTab = symTab;
		this.literalTab = literalTab;
		this.instTab = instTab;
	}

	/**
	 * 일반 문자열을 받아서 Token단위로 분리시켜 tokenList에 추가한다.
	 * 
	 * @param line : 분리되지 않은 일반 문자열
	 * @throws Exception
	 */
	public void putToken(String line) throws Exception {
		if (line.charAt(0) == '.')
			return;
		Token token = new Token(line);
		/** 심볼 테이블 넣는 과정 <- 라벨 처리 */
		if (token.label.length() > 0) {
			if (this.symTab.search(token.label) == -1) {
				this.symTab.putName(token.label, locctr);
			}
		}
		/** 리터럴 테이블 넣는 과정 <- =C'..', =X'..' 처리 */
		if (token.operand!=null && !token.operand[0].equals("") 
				&& token.operand[0].charAt(0) == '=') {
			/**
			 * 기존 C 코드에서는 type 변수를 추가정의하여 16진수인지 문자인지 판단했으나 
			 * 여기서는 literal에 =C'..', =X'..' 의 형태 그대로 넣어서 사용.
			 * 
			 */
			if (this.literalTab.search(token.operand[0]) == -1) {
				this.literalTab.putName(token.operand[0], 0); //주소값이 정해지지 않았으므로 0으로 추가한다.
			}
		}
		if (instTab.isDirectives(token.operator)) {
			token.byteSize = 0;
			if (token.operator.equals("RESW")) {
				token.location = locctr;
				int nums = Integer.parseInt(token.operand[0]);
				locctr += nums * 3;
			} else if (token.operator.equals("RESB")) {
				token.location = locctr;
				int nums = Integer.parseInt(token.operand[0]);
				locctr += nums;
			} else if (token.operator.equals("EXTDEF") 
					|| token.operator.equals("EXTREF")) {
				token.location = -1; // 문제 발생 여지 있음.
			} else if (token.operator.equals("LTORG") 
					|| token.operator.equals("END")) {
				token.location = -1;
				for (String s : this.literalTab.label) {
					String literal_temp = "\t*\t";
					literal_temp += s;
					putToken(literal_temp);
				}
			} else if (token.operator.equals("*")) {
				int len = token.operand[0].length() - 4;
				if (token.operand[0].charAt(1) == 'X')
					len /= 2;
				token.byteSize = len;
				token.location = locctr;
				this.literalTab.modifyName(token.operand[0], locctr);
				locctr += len;
			} else if (token.operator.equals("CSECT") 
					|| token.operator.equals("START")) {
				token.location = locctr;

			} else if (token.operator.equals("EQU")) {
				int templocctr=locctr;
				if (token.operand[0].charAt(0) != '*') {
					char op = '\0';
					for (int i = 0; i < token.operand[0].length(); i++) {
						if (token.operand[0].charAt(i) == '+' || token.operand[0].charAt(i) == '-') {
							op = token.operand[0].charAt(i);
						}
					}
					if (op != '\0') {
						String[] operation = token.operand[0].split("\\+|\\-");
						int v1 = this.symTab.search(operation[0]);
						int v2 = this.symTab.search(operation[1]);
						if (op == '+')
							templocctr = v1 + v2;
						else
							templocctr = v1 - v2;
					}
				}
				token.location = templocctr;
				this.symTab.modifyName(token.label, templocctr);
			} else if (token.operator.equals("BYTE")) {
				token.location = locctr;
				if (token.operand[0].charAt(0) == 'C') {
					locctr += token.operand[0].length() - 3;
				} else
					locctr += (token.operand[0].length() - 3) / 2;
			} else if (token.operator.equals("WORD")) {
				token.location = locctr;
				locctr += 3;
			}
		} else {
			token.byteSize = instTab.getFormat(token.operator);
			token.location = locctr;
			locctr += token.byteSize;
		}

		tokenList.add(token);
	}

	/**
	 * tokenList에서 index에 해당하는 Token을 리턴한다.
	 * 
	 * @param index
	 * @return : index번호에 해당하는 코드를 분석한 Token 클래스
	 */
	public Token getToken(int index) {
		return tokenList.get(index);
	}

	/**
	 * tokenList의 크기를 리턴한다.
	 * 
	 * @return : tokenList의 크기
	 */
	public int getTableSize() {
		return tokenList.size();
	}

	/**
	 * Pass2 과정에서 사용한다. instruction table, symbol table 등을 참조하여 objectcode를 생성하고, 이를
	 * 저장한다.
	 * 
	 * @param index
	 * @throws Exception
	 */
	public void makeObjectCode(int index) throws Exception {
		Token t = getToken(index);
		int object = 0; // Integer.toHexString으로 바꾸기 전 비트연산용
		if (t.operator == null && t.operator == "")
			return;
		/* 지시어에 대한 처리 시작 */
		if (this.instTab.isDirectives(t.operator)) {
			if (t.operator.equals("BYTE")) {
				String temp = t.operand[0].substring(2, t.operand[0].length() - 1);
				t.objectCode = temp;
				t.byteSize = temp.length() / 2;
			} else if (t.operator.equals("WORD")) {
				t.byteSize = 3;
				// operand가 숫자일 때 그대로 넣어주면 됨.
				if (t.operand[0].charAt(0) >= '0' && t.operand[0].charAt(0) <= '9')
					t.objectCode = t.operand[0];
				// 문자일 경우
				else {
					char op = '\0';
					for (int i = 0; i < t.operand[0].length(); i++) {
						if (t.operand[0].charAt(i) == '+' || t.operand[0].charAt(i) == '-') {
							op = t.operand[0].charAt(i);
						}
					}
					String[] operation = t.operand[0].split("\\+|\\-");
					int v1 = this.symTab.search(operation[0]);
					if (v1 == -1)
						v1 = 0;
					int v2 = this.symTab.search(operation[1]);
					if (v2 == -1)
						v2 = 0;
					if (op == '+')
						object = v1 + v2;
					else
						object = v1 - v2;
					String temp = Integer.toHexString(object).toUpperCase();
					t.objectCode = padding(temp, t.byteSize);
				}
			} else if (t.operator.equals("*")) {
				if (t.operand[0].charAt(1) == 'C') {
					for (int j = 3; j < t.operand[0].length() - 1; j++) {
						object <<= 8;
						object |= t.operand[0].charAt(j);
					}
					t.objectCode = Integer.toHexString(object).toUpperCase();
				} else if (t.operand[0].charAt(1) == 'X') {
					String temp = t.operand[0].substring(3, t.operand[0].length() - 1);
					t.objectCode = temp;
				}
			}
		}
		/* 일반 명령어의 처리 */
		else {
			object = this.instTab.getOpCode(t.operator);

			switch (t.byteSize) {
			case 1:
				break;
			case 2:
				for (int j = 0; j < t.operand.length; j++) {
					object <<= 4;
					if (j == 1 && (t.operand[j] == "" || t.operand[j] == null))
						object |= 0;
					else if (t.operand[j].equals("SW"))
						object |= 9;
					else if (t.operand[j].equals("PC"))
						object |= 8;
					else if (t.operand[j].equals("F"))
						object |= 6;
					else if (t.operand[j].equals("T"))
						object |= 5;
					else if (t.operand[j].equals("S"))
						object |= 4;
					else if (t.operand[j].equals("B"))
						object |= 3;
					else if (t.operand[j].equals("L"))
						object |= 2;
					else if (t.operand[j].equals("X"))
						object |= 1;
					else if (t.operand[j].equals("A"))
						object |= 0;
				}
				if (t.operand.length == 1)
					object <<= 4;
				break;
			case 3:
				object <<= 4;
				if (!t.operand[0].equals("")) { // null 포인터 exception 방지용.
					/* n, i flag setting */
					if (t.operand[0].charAt(0) == '@')
						t.setFlag(nFlag, 1);
					else if (t.operand[0].charAt(0) == '#')
						t.setFlag(iFlag, 1);
					else {
						t.setFlag(nFlag, 1);
						t.setFlag(iFlag, 1);
					}
				} else {
					t.setFlag(nFlag, 1);
					t.setFlag(iFlag, 1);
				}
				/* X flag setting */
				for (String op_iter : t.operand) {
					if (op_iter.equals("X")) {
						t.setFlag(xFlag, 1);
						break;
					}
				}

				if (!t.operand[0].equals("") && t.operand[0] != null) { // operand가 있는 경우
					if (t.operand[0].charAt(0) == '#') {// immediate addressing
						object |= t.nixbpe;
						object <<= 12;
						String tmp = t.operand[0].substring(1);
						object |= Integer.parseInt(tmp);
					} else {
						t.setFlag(pFlag, 1);
						object |= t.nixbpe;
						object <<= 12;
						int pc = t.location + t.byteSize;
						int target_addr;

						if (t.operand[0].charAt(0) == '=') // literal일 경우 literal tab 찾아야 함
							target_addr = this.literalTab.search(t.operand[0]);
						else if (t.operand[0].charAt(0) == '@')// Indirect addressing인 경우 operand 맨 앞의 @ 빼고 찾아야함.
							target_addr = this.symTab.search(t.operand[0].substring(1));
						else // Simple addressing인 경우 그냥 symbol table 찾으면 됨.
							target_addr = this.symTab.search(t.operand[0]);

						if (target_addr == -1)
							target_addr = 0;
						int disp = target_addr - pc;
						disp &= 0xFFF;
						object |= disp;
					}
				} else { // operand가 없는 경우. b,p,e flag 모두 0이고 disp도 없으므로 그냥 더함.
					object |= t.nixbpe;
					object <<= 12;
				}
				break;
			case 4:
				object <<= 4;
				/*4형식 명령어(e=1) 특성상 simple addressing이고(n,i=1), base나 pc를 참조할 수 없기때문에 (→주소 모름 || b,p=0)
				 * n,i,e 플래그를 우선적으로 1로 세팅하고, X는 사용 여부를 판단하여 세팅한다. 
				 */
				t.setFlag(nFlag, 1);
				t.setFlag(iFlag, 1);
				t.setFlag(eFlag, 1);
				for (String op_iter : t.operand) {
					if (op_iter.equals("X")) {
						t.setFlag(xFlag, 1);
						break;
					}
				}
				object |= t.nixbpe;
				object <<= 20;
				int target_addr = this.symTab.search(t.operand[0]);
				if (target_addr == -1)
					target_addr = 0;
				target_addr &= 0xFFFFF;
				object |= target_addr;
			} // end of switch
			t.objectCode = Integer.toHexString(object).toUpperCase();
			t.objectCode = padding(t.objectCode, t.byteSize);
		}
	}

	/**
	 * objectCode의 앞에 0으로 패딩을 넣어준다.
	 * 
	 * @param objectCode : 저장된 주소값
	 * @param byteSize   : objectCode가 가져야하는 바이트 크기
	 * @return byteSize에 맞게 0으로 패딩을 넣은 objectCode
	 */
	private String padding(String objectCode, int byteSize) {
		int len = objectCode.length();
		String ans = new String();
		while (len < byteSize * 2) {
			ans += "0";
			len++;
		}
		ans += objectCode;
		return ans.toUpperCase();
	}

	/**
	 * index번호에 해당하는 object code를 리턴한다.
	 * 
	 * @param index
	 * @return : object code
	 */
	public String getObjectCode(int index) {
		return tokenList.get(index).objectCode;
	}

}

/**
 * 각 라인별로 저장된 코드를 단어 단위로 분할한 후 의미를 해석하는 데에 사용되는 변수와 연산을 정의한다. 의미 해석이 끝나면 pass2에서
 * object code로 변형되었을 때의 바이트 코드 역시 저장한다.
 */
class Token {
	// 의미 분석 단계에서 사용되는 변수들
	int location; // <<- C 프로그램에서 locctr_table 역할
	String label;
	String operator;
	String[] operand;
	String comment;
	char nixbpe;

	// object code 생성 단계에서 사용되는 변수들
	String objectCode;
	int byteSize;

	/**
	 * 클래스를 초기화 하면서 바로 line의 의미 분석을 수행한다.
	 * 
	 * @param line 문장단위로 저장된 프로그램 코드
	 */
	public Token(String line) {
		// initialize ???
		parsing(line);
	}

	/**
	 * line의 실질적인 분석을 수행하는 함수. Token의 각 변수에 분석한 결과를 저장한다.
	 * 
	 * @param line 문장단위로 저장된 프로그램 코드.
	 */
	public void parsing(String line) {
		String[] line_temp = line.split("\t");
		switch (line_temp.length) {
		case 4:
			comment = line_temp[3];
		case 3:
			operand = line_temp[2].split(",");
		case 2:
			operator = line_temp[1];
		case 1:
			label = line_temp[0];
		}

	}

	/**
	 * n,i,x,b,p,e flag를 설정한다.
	 * 
	 * 
	 * 사용 예 : setFlag(nFlag, 1) 또는 setFlag(TokenTable.nFlag, 1)
	 * 
	 * @param flag  : 원하는 비트 위치
	 * @param value : 집어넣고자 하는 값. 1또는 0으로 선언한다.
	 */
	public void setFlag(int flag, int value) {
		// ...
		if (value == 1)
			nixbpe |= flag;
		else
			nixbpe &= ~flag;
	}

	/**
	 * 원하는 flag들의 값을 얻어올 수 있다. flag의 조합을 통해 동시에 여러개의 플래그를 얻는 것 역시 가능하다.
	 * 
	 * 사용 예 : getFlag(nFlag) 또는 getFlag(nFlag|iFlag)
	 * 
	 * @param flags : 값을 확인하고자 하는 비트 위치
	 * @return : 비트위치에 들어가 있는 값. 플래그별로 각각 32, 16, 8, 4, 2, 1의 값을 리턴할 것임.
	 */

	public int getFlag(int flags) {
		return nixbpe & flags;
	}
}
