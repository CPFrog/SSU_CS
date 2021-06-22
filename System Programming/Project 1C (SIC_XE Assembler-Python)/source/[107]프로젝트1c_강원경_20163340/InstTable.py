class Instruction:
    ''' 각각의 명령어에 대한 정보가 담기게 되는 class 구조체.
    명령어에 대한 정보를 저장하고, 기초적인 연산을 제공한다.'''

    def __init__(self, line: str = None):
        self.instruction: str = ''
        self.opcode: int = 0
        self.numberOfOperand: int = 0
        self.format: int = 0
        if line:
            self.parsing(line)
        else:
            pass

    def parsing(self, line: str):
        """ 문자열을 파싱하여 instruction 정보를 파악하고 저장한다.

            line:
                instruction 명세 파일로부터 한 줄씩 가져온 문자열
        """
        line_separate = line.split('\t')
        self.instruction = line_separate[0]
        self.format = int(line_separate[1])
        self.opcode = int(line_separate[2], 16)
        self.numberOfOperand = int(line_separate[3])


class InstTable:
    '''
    모든 instruction의 정보를 관리하는 클래스. instruction data들을 저장한다.
    또한 instruction 관련 연산, 예를 들면 목록을 구축하는 함수, 관련 정보를 제공하는 함수 등을 제공 한다.
    '''
    MAX_INST = 256
    directives = ["START", "END", "BYTE", "WORD", "RESB", "RESW",
                  "EXTDEF", "EXTREF", "LTORG", "*", "EQU", "CSECT"]
    instMap = {}  # 파이썬에서는 Map 형식이 아닌 Dictionary 방식으로 저장한다. 기능은 비슷함.

    def __init__(self, instFile=None):
        if instFile:
            self.openFile(instFile)
        else:
            pass

    def openFile(self, fileName: str):
        """ 입력받은 이름의 파일을 열고 그 내용을 parsing하여 instMap에 저장한다.

            fileName:
                 명령어 정보가 담긴 파일 이름
        """
        file = open(fileName)
        line = file.readlines()
        for s in line:
            inst = Instruction(s)
            self.instMap[inst.instruction] = inst
        file.close()

    def isDirectives(self, name: str) -> bool:
        """ 입력받은 명령어가 지시어 또는 예약어(ex. 선언자 '*') 인지 확인하는 함수.

            name:
                판단하려는 명령어의 이름
            Return:
                지시어/예약어인 경우 true, 아닌 경우 false
        """
        tempName = self.removeNewLine(name)
        for s in self.directives:
            if tempName == s:
                return True
        return False

    def nameToKey(self, name: str) -> str:
        """ 입력받은 string에서 '+' 문자를 제거하는 함수

            name:
                검색중 오류를 발생시킬 수 있는 특수문자가 포함된 string
            Return:
                특수문자가 제거된 string
        """
        return name.replace('+', '')

    def removeNewLine(self, name: str) -> str:
        """ 입력받은 string에서 개행 문자를 제거하는 함수

            name:
                검색중 오류를 발생시킬 수 있는 개행문자가 포함된 string
            Return:
                개행문자가 제거된 string
        """
        return name.replace('\n', '')

    def getFormat(self, name: str) -> int:
        """ 입력받은 이름을 가진 명령어가 몇 형식인지 반환하는 함수.

            name:
                검색하려는 명령어의 이름
            Return:
                명령어의 format
            Exception:
                해당 명령어가 유효하지 않은 명령어인 경우 Exception 발생
        """
        if name[0] == '+':
            return 4
        if not self.exists(name):
            raise Exception("없는 명령어입니다.")
        return self.search(name).format

    def getOpCode(self, name: str) -> int:
        """ 입력받은 이름을 가진 명령어의 opcode를 반환하는 함수.

            name:
                검색하려는 명령어의 이름
            Return:
                명령어의 opcode
            Exception:
                해당 명령어가 유효하지 않은 명령어인 경우 Exception 발생
        """
        if not self.exists(name):
            raise Exception("없는 명령어입니다.")
        return self.search(name).opcode

    def exists(self, name: str):
        """ 입력받은 이름을 가진 명령어가 instTable에 정의되어있는지 확인하는 함수.

            name:
                검색하려는 명령어의 이름
            Return:
                명령어 테이블에 있는 경우 true, 없는 경우 false
        """
        return self.nameToKey(self.removeNewLine(name)) in self.instMap

    def search(self, name: str):
        """ 입력받은 이름을 가진 명령어에 대한 정보를 반환하는 함수.

            name:
                검색하려는 명령어의 이름
            Return:
                그 명령어의 정보가 담긴 Instruction 구조체
        """
        return self.instMap.get(self.nameToKey(name))
