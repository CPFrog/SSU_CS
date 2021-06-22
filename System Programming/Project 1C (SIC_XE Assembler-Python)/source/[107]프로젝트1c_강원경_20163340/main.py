from Assembler import *

# 어셈블러의 메인 루틴을 수행하며, 루틴은 아래와 같이 수행된다.
assembler = Assembler("inst.data")
assembler.loadInputFile('input.txt')
assembler.pass1()

assembler.printSymbolTable('symtab_20163340')
assembler.printLiteralTable('literaltab_20163340')

assembler.pass2()
assembler.printObjectCode('output_20163340')
