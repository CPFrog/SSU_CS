from LabelTable import *
from TokenTable import *
from InstTable import *
import re


class Assembler:
    ''' 어셈블러의 역할을 수행하는 class이다. '''
    sector = -1

    def __init__(self, instFile: str = None):
        if instFile:
            self.lineList = list()
            self.symtabList = list()
            self.literaltabList = list()
            self.TokenList = []
            self.instTable = InstTable(instFile)
        else:
            pass

    def loadInputFile(self, inputFile: str = None):
        """ inputFile을 읽어 한 줄씩 lineList에 저장한다.

            inputFile:
                소스코드가 저장된 입력 파일 이름.
        """
        file = open(inputFile)
        self.lineList = file.readlines()
        file.close()

    def pass1(self):
        ''' pass1 과정을 수행하는 함수 '''
        pattern = re.compile('.*START|CSECT.*')
        for line in self.lineList:
            if pattern.search(line):
                self.sector += 1
                self.symtabList.append(LabelTable())
                self.literaltabList.append(LabelTable())
                self.TokenList.append(TokenTable(self.symtabList[self.sector],
                                                 self.literaltabList[self.sector], self.instTable))
            self.TokenList[self.sector].putToken(line)

    def printSymbolTable(self, fileName: str):
        """ 작성된 SymbolTable들을 출력 형태에 맞게 출력한다.

            fileName:
                SymbolTable을 저장하려는 파일 이름
        """
        file = open(fileName, 'w', encoding='utf-8')
        for i in range(self.sector + 1):
            size = len(self.TokenList[i].symTab.label)
            for j in range(size):
                s: str = ''
                s += self.TokenList[i].symTab.label[j]
                s += '\t'
                s += (str(hex(self.TokenList[i].symTab.locationList[j])).replace('0x', '', 1)).upper()
                file.write(s + '\n')
        file.close()

    def printLiteralTable(self, fileName: str):
        """ 작성된 LiteralTable들을 출력 형태에 맞게 출력한다.

            fileName:
                LiteralTable을 저장하려는 파일 이름
        """
        file = open(fileName, 'w', encoding='utf-8')
        for i in range(self.sector + 1):
            size = len(self.TokenList[i].literalTab.label)
            for j in range(size):
                s: str = ''
                s += self.TokenList[i].literalTab.label[j][3:-1]
                s += '\t'
                s += (str(hex(self.TokenList[i].literalTab.locationList[j])).replace('0x', '', 1)).upper()
                file.write(s + '\n')
        file.close()

    def pass2(self):
        ''' pass2 과정을 수행하는 함수 '''
        for t in self.TokenList:
            for i in range(t.getTableSize()):
                t.makeObjectCode(i)

    def padding(self, objectCode: str, byteSize: int) -> str:
        """ objectCode의 앞에 0으로 패딩을 넣어준다.

            objectCode:
                패딩을 넣으려고 하는 string형태의 objectCode
            byteSize:
                objectCode가 가져야 하는 바이트 크기
            Return:
                byteSize에 맞게 0으로 패딩을 넣은 string 형태의 objectCode
        """
        length = len(objectCode)
        ans = ''
        while length < byteSize * 2:
            ans += '0'
            length += 1
        ans += objectCode
        return ans.upper()

    def padding_int(self, objectCode_int: int, byteSize: int) -> str:
        """ objectCode의 앞에 0으로 패딩을 넣어준다.

            objectCode:
                패딩을 넣으려고 하는 int 형태의 objectCode
            byteSize:
                objectCode가 가져야 하는 바이트 크기
            Return:
                byteSize에 맞게 0으로 패딩을 넣은 string 형태의 objectCode
        """
        temp = str(hex(objectCode_int)).replace('0x', '', 1)
        return self.padding(temp, byteSize)

    def writeBuftoFile(self, bufWriter: str, file):
        """ 한 줄의 object code가 저장된 string을 파일에 기록하는 함수.

            bufWriter:
                한 줄의 objectCode가 저장된 string 형태의 버퍼
            file:
                내용을 기록하려는 파일
        """
        line_len = (len(bufWriter) - 9) // 2
        file.write(bufWriter[:7])
        file.write(self.padding_int(line_len, 1))
        file.write(bufWriter[9:])
        file.write('\n')

    def printObjectCode(self, fileName: str):
        """ 작성된 objectCode들을 모아 하나의 objectProgram을 만든다.

            file:
                내용을 기록하려는 파일
        """
        file = open(fileName, 'w', encoding='utf-8')
        cur_buf_length = 0
        cur_sector_num = 0

        bufWriter = ''

        # Control Section별 토큰테이블 호출
        for table in self.TokenList:
            # H record 작성
            bufWriter += 'H'
            bufWriter += table.symTab.label[0] + '\t'
            bufWriter += self.padding_int(table.symTab.locationList[0], 3)
            bufWriter += self.padding_int(table.locctr, 3) + '\n'
            file.write(bufWriter)
            bufWriter = ''

            # EXTREF의 operand를 저장하는 String 배열
            ref_ops = list()

            # EXTDEF, EXTREF 처리
            for t in table.tokenList:
                if t.byteSize == 0:
                    # EXTDEF 처리
                    if t.operator == 'EXTDEF':
                        bufWriter = 'D'
                        cur_buf_length = 1
                        for s in t.operand:
                            if cur_buf_length >= 57:
                                bufWriter += '\n'
                                file.write(bufWriter)
                                bufWriter = 'D'
                                cur_buf_length = 1
                            bufWriter += s
                            for i in range(len(s), 6):
                                bufWriter += ' '
                            addr: int = table.symTab.search(s)
                            bufWriter += self.padding_int(addr, 3)
                            cur_buf_length += len(s) + 6
                        bufWriter += '\n'
                        file.write(bufWriter)
                        bufWriter = ''
                        cur_buf_length = 0
                    # EXTREF 처리
                    elif t.operator == 'EXTREF':
                        ref_ops = t.operand
                        bufWriter = 'R'
                        cur_buf_length = 1
                        for s in t.operand:
                            if cur_buf_length >= 57:
                                bufWriter += '\n'
                                file.write(bufWriter)
                                bufWriter = 'R'
                                cur_buf_length = 1
                            bufWriter += s
                            for i in range(len(s), 6):
                                bufWriter += ' '
                            cur_buf_length += 6
                        bufWriter += '\n'
                        file.write(bufWriter)
                        bufWriter = ''
                        cur_buf_length = 0
                        break

            is_newline = True

            # T record 작성
            for t in table.tokenList:
                if is_newline:
                    bufWriter = 'T'
                    bufWriter += self.padding_int(t.location, 3)
                    bufWriter += '00'
                    is_newline = False

                if not self.instTable.isDirectives(t.operator):
                    if len(bufWriter) + t.byteSize * 2 > 69:
                        self.writeBuftoFile(bufWriter, file)
                        bufWriter = 'T'
                        bufWriter += self.padding_int(t.location, 3)
                        bufWriter += '00'
                    bufWriter += self.padding(t.objectCode, t.byteSize)

                # 리터럴 정의 부분에 대한 처리
                else:
                    if t.operator == '*':
                        self.writeBuftoFile(bufWriter, file)
                        bufWriter = 'T'
                        bufWriter += self.padding_int(t.location, 3)
                        bufWriter += '03'
                        bufWriter += self.padding(t.objectCode, t.byteSize)

                    elif t.operator == 'WORD' or t.operator == 'BYTE':
                        bufWriter += t.objectCode

            self.writeBuftoFile(bufWriter, file)

            # M record 작성
            for t in table.tokenList:
                if t.operand is not None:
                    for line_op in t.operand:
                        for ext_op in ref_ops:
                            i = line_op.find(ext_op)
                            if i != -1 and t.location != -1:
                                if self.instTable.exists(t.operator):
                                    bufWriter = ('M' + self.padding_int(t.location + 1, 3))
                                    bufWriter += '05+'
                                else:
                                    bufWriter = ('M' + self.padding_int(t.location, 3))
                                    if i > 0 and line_op[i - 1] == '-':
                                        bufWriter += '06-'
                                    else:
                                        bufWriter += '06+'
                                bufWriter += (ext_op + '\n')
                                file.write(bufWriter)
                                bufWriter = ''

            # E record 작성
            if cur_sector_num == 0:
                bufWriter = 'E'
                bufWriter += self.padding_int(table.symTab.locationList[0], 3)
                bufWriter += '\n\n'
            else:
                bufWriter = 'E\n\n'
            file.write(bufWriter)
            bufWriter = ''
            cur_sector_num += 1
        file.close()
