import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/**
 * Assembler: 이 프로그램은 SIC/XE 머신을 위한 Assembler 프로그램의 메인루틴이다. 프로그램의 수행 작업은 다음과 같다.
 * 1) 처음 시작하면 Instruction 명세를 읽어들여서 assembler를 세팅한다.
 * 
 * 2) 사용자가 작성한 input 파일을 읽어들인 후 저장한다
 * 
 * 3) input 파일의 문장들을 단어별로 분할하고 의미를 파악해서 정리한다. (pass1)
 * 
 * 4) 분석된 내용을바탕으로 컴퓨터가 사용할 수 있는 object code를 생성한다. (pass2)
 * 
 * 
 * 작성중의 유의사항:
 * 
 * 1) 새로운 클래스, 새로운 변수, 새로운 함수 선언은 얼마든지 허용됨. 단, 기존의 변수와 함수들을 삭제하거나 완전히 대체하는 것은
 * 안된다.
 * 
 * 2) 마찬가지로 작성된 코드를 삭제하지 않으면 필요에 따라 예외처리, 인터페이스 또는 상속 사용 또한 허용됨
 * 
 * 3) 모든 void 타입의 리턴값은 유저의 필요에 따라 다른 리턴 타입으로 변경 가능.
 * 
 * 4) 파일, 또는 콘솔창에 한글을 출력시키지말 것. (채점상의 이유. 주석에 포함된 한글은 상관 없음)
 * 
 * + 제공하는 프로그램 구조의 개선방법을 제안하고 싶은 분들은 보고서의 결론 뒷부분에 첨부 바랍니다. 내용에 따라 가산점이 있을 수
 * 있습니다.
 */

public class Assembler {
	/** instruction 명세를 저장한 공간 */
	InstTable instTable;
	/** 읽어들인 input 파일의 내용을 한 줄 씩 저장하는 공간. */
	ArrayList<String> lineList;
	/** 프로그램의 section별로 symbol table을 저장하는 공간 */
	ArrayList<LabelTable> symtabList;
	/** 프로그램의 section별로 literal table을 저장하는 공간 */
	ArrayList<LabelTable> literaltabList;
	/** 프로그램의 section별로 프로그램을 저장하는 공간 */
	ArrayList<TokenTable> TokenList;
	/**
	 * Token, 또는 지시어에 따라 만들어진 오브젝트 코드들을 출력 형태로 저장하는 공간. 필요한 경우 String 대신 별도의 클래스를
	 * 선언하여 ArrayList를 교체해도 무방함.
	 */
	ArrayList<String> codeList;

	int sector = -1;

	/**
	 * 클래스 초기화. instruction Table을 초기화와 동시에 세팅한다.
	 * 
	 * @param instFile : instruction 명세를 작성한 파일 이름.
	 */
	public Assembler(String instFile) {
		instTable = new InstTable(instFile);
		lineList = new ArrayList<String>();
		symtabList = new ArrayList<LabelTable>();
		literaltabList = new ArrayList<LabelTable>();
		TokenList = new ArrayList<TokenTable>();
		codeList = new ArrayList<String>();
	}

	/**
	 * 어셈블러의 메인 루틴
	 */
	public static void main(String[] args) {
		try {
			Assembler assembler = new Assembler("inst.data");
			assembler.loadInputFile("input.txt");
			assembler.pass1();

			assembler.printSymbolTable("symtab_20163340");
			assembler.printLiteralTable("literaltab_20163340");
			assembler.pass2();
			assembler.printObjectCode("output_20163340");
		} catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}

	/**
	 * inputFile을 읽어들여서 lineList에 저장한다.
	 * 
	 * @param inputFile : input 파일 이름.
	 */
	private void loadInputFile(String inputFile) { // init_input_file
		try {
			File file = new File(inputFile);
			FileReader filereader = new FileReader(file);
			BufferedReader bufReader = new BufferedReader(filereader);
			String line = "";
			while ((line = bufReader.readLine()) != null) {
				lineList.add(line);
			}
			bufReader.close();
		} catch (FileNotFoundException e) {
			System.out.println(e);
		} catch (IOException e) {
			System.out.println(e);
		}
	}

	/**
	 * pass1 과정을 수행한다.
	 * 
	 * 1) 프로그램 소스를 스캔하여 토큰 단위로 분리한 뒤 토큰 테이블을 생성.
	 * 
	 * 2) symbol, literal 들을 SymbolTable, LiteralTable에 정리.
	 * 
	 * 주의사항: SymbolTable, LiteralTable, TokenTable은 프로그램의 section별로 하나씩 선언되어야 한다.
	 * 
	 * @param inputFile : input 파일 이름.
	 * @throws Exception
	 */

	private void pass1() throws Exception {
		for (String line : lineList) {
			if (line.matches(".*(START|CSECT).*")) { // 새로 섹터 시작되면 새로운 테이블들 생성
				sector++;
				symtabList.add(new LabelTable());
				literaltabList.add(new LabelTable());
				TokenList.add(new TokenTable(symtabList.get(sector), literaltabList.get(sector), instTable));
			}
			TokenList.get(sector).putToken(line);
		}
	}

	/**
	 * 작성된 SymbolTable들을 출력형태에 맞게 출력한다.
	 * 
	 * @param fileName : 저장되는 파일 이름
	 */
	private void printSymbolTable(String fileName) {
		try {
			File file = new File(fileName);
			FileWriter filewriter = new FileWriter(file);
			BufferedWriter bufWriter = new BufferedWriter(filewriter);

			for (int i = 0; i <= sector; i++) {
				int size = TokenList.get(i).symTab.label.size();
				for (int j = 0; j < size; j++) {
					bufWriter.write(TokenList.get(i).symTab.label.get(j));
					bufWriter.write('\t');
					bufWriter.write(Integer.toHexString(TokenList.get(i).symTab.locationList.get(j)).toUpperCase());
					bufWriter.write('\n');
					bufWriter.flush();
				}
				bufWriter.write('\n');
				bufWriter.flush();
			}
			bufWriter.close();

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 작성된 LiteralTable들을 출력형태에 맞게 출력한다.
	 * 
	 * @param fileName : 저장되는 파일 이름
	 */
	private void printLiteralTable(String fileName) {
		try {
			File file = new File(fileName);
			FileWriter filewriter = new FileWriter(file);
			BufferedWriter bufWriter = new BufferedWriter(filewriter);

			for (int i = 0; i <= sector; i++) {
				int size = TokenList.get(i).literalTab.label.size();
				for (int j = 0; j < size; j++) {
					String literal_temp = TokenList.get(i).literalTab.label.get(j);
					bufWriter.write(literal_temp.substring(3, literal_temp.length() - 1));
					bufWriter.write('\t');
					bufWriter.write(Integer.toHexString(TokenList.get(i).literalTab.locationList.get(j)).toUpperCase());
					bufWriter.write('\n');
					bufWriter.flush();
				}
				bufWriter.write('\n');
				bufWriter.flush();
			}
			bufWriter.close();

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	
	/**
	 * pass2 과정을 수행한다.
	 * 
	 * 1) 분석된 내용을 바탕으로 object code를 생성하여 codeList에 저장.
	 * 
	 * @throws Exception
	 */
	private void pass2() throws Exception {
		for (TokenTable t : TokenList) {
			for (int i = 0; i < t.getTableSize(); i++) {
				t.makeObjectCode(i);
				codeList.add(t.getObjectCode(i));
			}
		}
	}
	
	
	/**
	 * 작성된 codeList를 출력형태에 맞게 출력한다.
	 * --> 실제 구현 : codeList를 사용하지 않고, Token의 멤버변수인 objectCode 사용.
	 * 	    └> 이유 : codeList로 ObjectProgram 작성 시 Control Section에 대한 구분 과정이 복잡해짐.
	 * @param fileName : 저장되는 파일 이름
	 */
	private void printObjectCode(String fileName) {
		try {
			File file = new File(fileName);
			FileWriter filewriter = new FileWriter(file);
			BufferedWriter bufWriter = new BufferedWriter(filewriter);

			int cur_buf_length = 0; // 버퍼 길이 확인용 변수. 한줄에는 최대 68개 문자까지 가능.
			int cur_sector_num = 0;
			/** Section별 table 가져오기 */
			for (TokenTable table : TokenList) {
				/* Control Section 시작할 때 Header 정보 기록 */
				bufWriter.write('H');
				// Control Section 이름
				bufWriter.write(table.symTab.label.get(0) + "\t");
				// Control Section 시작 주소
				bufWriter.write(padding(table.symTab.locationList.get(0), 3));
				// 프로그램 길이
				bufWriter.write(padding(table.locctr, 3) + "\n");
				bufWriter.flush();

				String[] ref_ops = new String[3];  //EXTREF의 operand를 저장하는 String 배열

				/* EXTDEF, EXTREF 처리 */
				for (Token t : table.tokenList) {
					// 특수한 operator에 대한 처리
					if (t.byteSize == 0) {
						/* EXTDEF 처리 */
						if (t.operator.equals("EXTDEF")) {
							bufWriter.write('D');
							cur_buf_length = 1;
							for (String s : t.operand) {
								if (cur_buf_length >= 57) {
									bufWriter.write('\n');
									bufWriter.flush();
									bufWriter.write('D');
									cur_buf_length = 1;
								}
								bufWriter.write(s);
								for (int i = s.length(); i < 6; i++)
									bufWriter.write(' ');
								int addr = table.symTab.search(s);
								bufWriter.write(padding(addr, 3));
								cur_buf_length += s.length() + 6;
							}
							bufWriter.write('\n');
							bufWriter.flush();
							cur_buf_length = 0;
						}
						/* EXTREF 처리 */
						else if (t.operator.equals("EXTREF")) {
							ref_ops = t.operand;
							bufWriter.write('R');
							cur_buf_length = 1;
							for (String s : t.operand) {
								if (cur_buf_length >= 57) {
									bufWriter.write('\n');
									bufWriter.flush();
									bufWriter.write('R');
									cur_buf_length = 1;
								}
								bufWriter.write(s);
								for (int i = s.length(); i < 6; i++)
									bufWriter.write(' ');
								cur_buf_length += 6;
							}
							bufWriter.write('\n');
							bufWriter.flush();
							cur_buf_length = 0;
							break;
						}

					}

				}

				boolean is_newline = true;
				/** 파일 쓰기 시 라인의 길이 정보 수정때문에 String 형태의 버퍼에 1차적으로 쓰고 
				 * 기록 전 subString을 활용한 줄 길이에 대한 내용 갱신 후 버퍼를 통해 파일 기록*/
				
				String Linebuffer = new String();
				
				/** T 부분 처리 */
				for (Token t : table.tokenList) {
					if (is_newline) {
						Linebuffer += 'T';
						Linebuffer += padding(t.location, 3);
						Linebuffer += "00";
						is_newline = false;
					}
					if (!instTable.isDirectives(t.operator)) {
						if (Linebuffer.length() + t.byteSize * 2 > 69) {
							writeBuftoFile(bufWriter,Linebuffer);
							Linebuffer = "T";
							Linebuffer += padding(t.location, 3);
							Linebuffer += "00";
						}
						Linebuffer += padding(t.objectCode, t.byteSize);
						// 리터럴 정의 부분에 대한 처리
					} else {
						if (t.operator.equals("*")) {
							writeBuftoFile(bufWriter,Linebuffer);
							Linebuffer = "T";
							Linebuffer += padding(t.location, 3);
							Linebuffer += "03";
							Linebuffer += padding(t.objectCode, t.byteSize);
						}
						else if(t.operator.equals("WORD")||t.operator.equals("BYTE")) {
							Linebuffer+=t.objectCode;
						}
					}
				}
				writeBuftoFile(bufWriter,Linebuffer);

				// M 처리
				for (Token t : table.tokenList) {
					if (t.operand != null) {
					for (String line_op : t.operand) {
							for (String ext_op : ref_ops) {
								int i = line_op.indexOf(ext_op);
								if (i != -1 && t.location != -1) {
									if (instTable.exists(t.operator)) {
										bufWriter.write("M" + padding(t.location + 1, 3));
										bufWriter.write("05+");
									} else {
										bufWriter.write("M" + padding(t.location, 3));
										if (i > 0 && line_op.charAt(i - 1) == '-')
											bufWriter.write("06-");
										else
											bufWriter.write("06+");
									}
									bufWriter.write(ext_op + "\n");
									bufWriter.flush();
								}
							}
						}
					}

				}

				// E 처리
				if (cur_sector_num == 0) {
					bufWriter.write("E");
					bufWriter.write(padding(table.symTab.locationList.get(0), 3));
					bufWriter.write("\n\n");
				} else
					bufWriter.write("E\n\n");
				bufWriter.flush();
				cur_sector_num++;
			}
			bufWriter.close();
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	/**
	 * byteSize에 맞도록 objectCode의 앞에 0으로 패딩을 넣어준다.
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
	 * int 형태의 objectCode를 String으로 바꾼 뒤 다시 padding 함수를 호출한다.
	 * 
	 * @param objectCode_int : integer 형태의 objectCode
	 * @param byteSize   : objectCode가 가져야하는 바이트 크기
	 * @return byteSize에 맞게 0으로 패딩을 넣은 objectCode
	 */
	private String padding(int objectCode_int, int byteSize) {
		String temp = Integer.toHexString(objectCode_int);
		return padding(temp, byteSize);
	}
	
	/**
	 * 한 줄의 object code가 저장된 문자열을 버퍼를 통해 파일에 기록하는 함수이다. 
	 * 
	 * @param bufWriter : 기록하려는 파일과 연결된 버퍼
	 * @param Linebuffer: 한 줄의 object code가 저장된 문자열 형태의 버퍼
	 */
	private void writeBuftoFile(BufferedWriter bufWriter,String Linebuffer) throws IOException {
		int line_len = (Linebuffer.length() - 9) / 2;
		bufWriter.write(Linebuffer.substring(0, 7)); // 0~6까지 버퍼 넣음
		bufWriter.write(padding(line_len, 1)); // 7~8까지는 String 길이 넣음 (길이 정보 갱신)
		bufWriter.write(Linebuffer.substring(9)); // 9~끝까지 버퍼 넣음
		bufWriter.write('\n');
		bufWriter.flush();
	}
}
