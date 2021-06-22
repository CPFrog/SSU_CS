from InstTable import *
from LabelTable import *
import re

MAX_OPERAND = 3
nFlag = 32
iFlag = 16
xFlag = 8
bFlag = 4
pFlag = 2
eFlag = 1


class Token:
    """ 각 라인별로 저장된 코드를 단어 단위로 분할한 후 의미를 해석하는 데에 사용되는 변수와 연산을 정의한다.
    의미 해석이 끝나면 pass2에서 object code로 변형되었을 때의 바이트 코드 역시 저장한다.
    """

    def __init__(self, line: str = None):
        self.location: int = 0
        self.label: str = None
        self.operator: str = None
        self.operand: str = None
        self.comment: str = None
        self.nixbpe = 0

        self.objectCode: str = None
        self.byteSize: int = 0
        if line is not None:
            self.parsing(line)

    def parsing(self, line):
        """ line을 분석하여 토큰화를 수행하는 함수.

            line:
                분석을 수행할 한 줄의 내용이 담긴 string
        """
        line_temp = line.split('\t')
        if len(line_temp) > 0:
            self.label = line_temp[0]
            if len(line_temp) > 1:
                self.operator = line_temp[1]
                if len(line_temp) > 2:
                    self.operand = line_temp[2].split(',')
                    if len(line_temp) > 3:
                        self.comment = line_temp[3]

    def setFlag(self, flag, value=1):
        """ n,i,x,b,p,e의 flag를 설정하는 함수.

            flag:
                설정하려는 flag의 이름
            value:
                플래그를 어떤 값으로 세팅할 것인지 지정. 값을 지정하지 않으면 1로 세팅한다.
        """
        self.nixbpe |= flag


class TokenTable:
    def __init__(self, symTab: LabelTable, literalTab: LabelTable, instTab: InstTable):
        self.symTab = symTab
        self.literalTab = literalTab
        self.instTab = instTab
        self.tokenList = []
        self.locctr: int = 0

    def getToken(self, index: int) -> Token:
        """ tokenTable의 index 번째에 위치에 있는 Token을 리턴

            index:
                찾으려는 토큰의 테이블에서의 인덱스 번호
            Return:
                index 번째에 위치한 토큰 정보 구조체
        """
        return self.tokenList[index]

    def getTableSize(self) -> int:
        """ tokenList의 크기 반환

            Return:
                tokenList의 크기
        """
        return len(self.tokenList)

    def getObjectCode(self, index: int) -> str:
        """ tokenTable의 index 번째에 위치한 Token의 ObjectCode를 반환

            index:
                찾으려는 토큰의 테이블에서의 인덱스 번호
            Return:
                토큰의 Object Code
        """
        return self.tokenList[index].objectCode

    def padding(self, objectCode: str, byteSize: int) -> str:
        """ objectCode의 앞에 0으로 패딩을 넣어준다.

            objectCode:
                패딩을 넣으려고 하는 string형태의 objectCode
            byteSize:
                objectCode가 가져야 하는 바이트 크기
            Return:
                byteSize에 맞게 0으로 패딩을 넣은 string 형태의 objectCode
        """
        length: int = len(objectCode)
        ans: str = ''
        while length < byteSize * 2:
            ans += '0'
            length += 1
        ans += objectCode
        return ans.upper()

    def putToken(self, line: str):
        """ 일반 문자열을 입력받아 Token 단위로 분리시켜 tokenList에 추가

            line:
                토큰화가 수행되기 전 문자열
        """
        if line[0] == '.':
            return
        token: Token = Token(line)
        # 심볼 테이블 넣는 과정. <- label 처리
        if len(token.label) > 0:
            if self.symTab.search(token.label) == -1:
                self.symTab.putName(token.label, self.locctr)
        # 리터럴 테이블 넣는 과정 <- =C'..', =X'..' 처리
        if token.operand != [''] and token.operand[0] != '' and token.operand[0][0] == '=':
            if self.literalTab.search(token.operand[0]) == -1:
                self.literalTab.putName(token.operand[0], 0)  # 리터럴의 주소값은 나중에 수정된다.
        # operator가 지시어일 경우 각 경우에 맞게 처리
        if self.instTab.isDirectives(token.operator):
            token.byteSize = 0
            if token.operator == 'RESW':
                token.location = self.locctr
                nums: int = int(token.operand[0])
                self.locctr += nums * 3
            elif token.operator == 'RESB':
                token.location = self.locctr
                nums: int = int(token.operand[0])
                self.locctr += nums
            elif token.operator == 'EXTDEF' or token.operator == 'EXTREF':
                token.location = -1
            elif token.operator == 'LTORG' or token.operator == 'END':
                token.location = -1
                for s in self.literalTab.label:
                    literal_temp = '\t*\t'
                    literal_temp += s
                    self.putToken(literal_temp)
            elif token.operator == '*':
                length = len(token.operand[0]) - 4
                if token.operand[0][1] == 'X':
                    length //= 2
                token.byteSize = length
                token.location = self.locctr
                self.literalTab.modifyName(token.operand[0], self.locctr)
                self.locctr += length
            elif token.operator == 'CSECT' or token.operator == 'START':
                token.location = self.locctr
            elif token.operator == 'EQU':
                templocctr = self.locctr
                if token.operand[0][0] != '*':
                    op = None
                    for i in range(0, len(token.operand[0])):
                        if token.operand[0][i] == '+' or token.operand[0][i] == '-':
                            op = token.operand[0][i]
                    if op is not None:
                        operation = re.split(r'[+-]', token.operand[0])
                        v1 = self.symTab.search(operation[0])
                        v2 = self.symTab.search(operation[1])
                        if op == '+':
                            templocctr = v1 + v2
                        else:
                            templocctr = v1 - v2
                token.location = templocctr
                self.symTab.modifyName(token.label, templocctr)
            elif token.operator == 'BYTE':
                token.location = self.locctr
                if token.operand[0][0] == 'C':
                    self.locctr += len(token.operand[0]) - 3
                else:
                    self.locctr += (len(token.operand[0]) - 3) // 2
            elif token.operator == 'WORD':
                token.location = self.locctr
                self.locctr += 3
        # operator가 일반적인 명령어일 경우 처리
        else:
            token.byteSize = self.instTab.getFormat(token.operator)
            token.location = self.locctr
            self.locctr += token.byteSize

        self.tokenList.append(token)

    """ pass2에서 objectCode를 만들 때 사용하는 함수. 
    명령어 테이블, 심볼 테이블, 리터럴 테이블을 참조하여 objectCode를 만들고 
    Token class 객체 내의 objectCode 변수에 저장.

        index:
            objectCode를 만드려는 토큰의 인덱스 번호
    """

    def makeObjectCode(self, index: int):
        t = self.getToken(index)
        obj: int = 0
        if t.operator is None and (t.operator == '' or t.operator is None):
            return
        # 지시어에 대한 처리 부분
        if self.instTab.isDirectives(t.operator):
            if t.operator == 'BYTE':
                temp = t.operand[0][2:-1]
                t.objectCode = temp
                t.byteSize = len(temp) // 2
                # print(t.objectCode + '\n') <-디버깅용
            elif t.operator == 'WORD':
                t.byteSize = 3
                # operand가 숫자라면 그대로 objectCode에 넣음
                if t.operand[0][0].isdigit():
                    t.objectCode = t.operand[0]
                # 문자일 경우 처리
                else:
                    op = None
                    for i in (0, len(t.operand[0]) - 1):
                        if t.operand[0][i] == '+' or t.operand[0][i] == '-':
                            op = t.operand[0][i]
                    operation = re.split(r'[+-]', t.operand[0])
                    v1: int = self.symTab.search(operation[0])
                    if v1 == -1:
                        v1 = 0
                    v2: int = self.symTab.search(operation[1])
                    if v2 == -1:
                        v2 = 0
                    if op == '+':
                        obj = v1 + v2
                    elif op == '-':
                        obj = v1 - v2
                    temp = hex(obj)
                    t.objectCode = self.padding(temp.replace('0x', '', 1), t.byteSize)
                    # print(t.objectCode + '\n')
            # 리터럴을 정의한 토큰일 경우
            elif t.operator == '*':
                if t.operand[0][1] == 'C':
                    for j in range(3, len(t.operand[0]) - 1):
                        obj <<= 8
                        obj |= ord(t.operand[0][j])
                    t.objectCode = (hex(obj).replace('0x', '', 1)).upper()
                elif t.operand[0][1] == 'X':
                    temp = t.operand[0][3:-1]
                    t.objectCode = temp
                # print(t.objectCode + '\n')
        # 일반적인 명령어일 경우의 처리 부분
        else:
            obj = self.instTab.getOpCode(t.operator)

            if t.byteSize == 2:
                for j in range(len(t.operand)):
                    obj <<= 4
                    if j == 1 and (t.operand[j] == '' or t.operand[j] is None):
                        obj |= 0
                    elif t.operand[j] == 'SW':
                        obj |= 9
                    elif t.operand[j] == 'PC':
                        obj |= 8
                    elif t.operand[j] == 'F':
                        obj |= 6
                    elif t.operand[j] == 'T':
                        obj |= 5
                    elif t.operand[j] == 'S':
                        obj |= 4
                    elif t.operand[j] == 'B':
                        obj |= 3
                    elif t.operand[j] == 'L':
                        obj |= 2
                    elif t.operand[j] == 'X':
                        obj |= 1
                    elif t.operand[j] == 'A':
                        obj |= 0
                if len(t.operand) == 1:
                    obj <<= 4
            elif t.byteSize == 3:
                obj <<= 4
                # n, i 플래그 세팅
                if t.operand[0] != '':
                    if t.operand[0][0] == '@':
                        t.setFlag(nFlag, 1)
                    elif t.operand[0][0] == '#':
                        t.setFlag(iFlag, 1)
                    else:
                        t.setFlag(nFlag, 1)
                        t.setFlag(iFlag, 1)
                else:
                    t.setFlag(nFlag, 1)
                    t.setFlag(iFlag, 1)

                # x 플래그 세팅
                for op_iter in t.operand:
                    if op_iter == 'X':
                        t.setFlag(xFlag, 1)
                        break

                # operand가 있는 경우
                if t.operand != [''] and t.operand[0] is not None:
                    # immediate addressing 일 때
                    if t.operand[0][0] == '#':
                        obj |= t.nixbpe
                        obj <<= 12
                        tmp: str = t.operand[0][1:]
                        obj |= int(tmp)
                    else:
                        t.setFlag(pFlag, 1)
                        obj |= t.nixbpe
                        obj <<= 12
                        pc: int = t.location + t.byteSize
                        target_addr: int

                        # operand가 literal일 때 리터럴 테이블을 검색한다.
                        if t.operand[0][0] == '=':
                            target_addr = self.literalTab.search(t.operand[0])
                        # 간접 주소 방식인 경우 '@'를 제외하고 심볼 테이블 검색
                        elif t.operand[0][0] == '@':
                            target_addr = self.symTab.search(t.operand[0][1:])
                        # simple addressing 인 경우 심볼 테이블을 찾기만 하면 됨.
                        else:
                            target_addr = self.symTab.search(t.operand[0])

                        if target_addr == -1:
                            target_addr = 0
                        disp: int = target_addr - pc
                        disp &= 0xFFF
                        obj |= disp
                # operand가 없는 경우 b,p,e 플래그 모두 0이고 disp도 없으므로 그냥 더한다.
                else:
                    obj |= t.nixbpe
                    obj <<= 12
            elif t.byteSize == 4:
                obj <<= 4
                ''' 
                4형식 명령어(e=1) 특성상 simple addressing이고(n,i=1), base나 pc를 참조할 수 없기때문에 (→주소 모름 || b,p=0)
                n,i,e 플래그를 우선적으로 1로 세팅하고, X는 사용 여부를 판단하여 세팅한다.
                '''
                t.setFlag(nFlag)
                t.setFlag(iFlag)
                t.setFlag(eFlag)

                for op_iter in t.operand:
                    if op_iter == 'X':
                        t.setFlag(xFlag)
                        break

                obj |= t.nixbpe
                obj <<= 20
                target_addr: int = self.symTab.search(t.operand[0])
                if target_addr == -1:
                    target_addr = 0
                target_addr &= 0xFFFFF
                obj |= target_addr

            t.objectCode = (hex(obj).replace('0x', '', 1)).upper()
            t.objectCode = self.padding(t.objectCode, t.byteSize)
            # print(t.objectCode + '\n')
