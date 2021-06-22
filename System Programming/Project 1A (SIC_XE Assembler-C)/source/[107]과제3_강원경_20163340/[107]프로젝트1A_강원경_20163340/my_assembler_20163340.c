#define _CRT_SECURE_NO_WARNINGS // �Ϻ� �Լ��� ���� ���� ���ÿ�
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "my_assembler_20163340.h"


/* ----------------------------------------------------------------------------------
 * ���� : ����ڷ� ���� ����� ������ �޾Ƽ� ��ɾ��� OPCODE�� ã�� ����Ѵ�.
 * �Ű� : ���� ����, ����� ����
 * ��ȯ : ���� = 0, ���� = < 0
 * ���� : ���� ����� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�.
 *		   ���� �߰������� �������� �ʴ´�.
 * ----------------------------------------------------------------------------------
 */
int main(int args, char* arg[]) {
	if (init_my_assembler() < 0) {
		printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n");
		return -1;
	}

	if (assem_pass1() < 0) {
		printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}

	//make_opcode_output("output_20163340");

	make_symtab_output("symtab_20163340");
	make_literaltab_output("literaltab_107");

	if (assem_pass2() < 0)
	{
		printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}
	make_objectcode_output("output_20163340");

	return 0;
}

/* ----------------------------------------------------------------------------------
 * ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�.
 * �Ű� : ����
 * ��ȯ : �������� = 0 , ���� �߻� = -1
 * ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ�
		���� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
		�����Ͽ���.
 * ----------------------------------------------------------------------------------
 */
int init_my_assembler(void) {
	int result = 0;
	if ((result = init_inst_file("inst.data")) < 0)
		return -1;
	if ((result = init_input_file("input.txt")) < 0)
		return -1;
	return result;
}


/* ----------------------------------------------------------------------------------
 * ���� : ��ɾ� ���� ����� inst.data ������ ���پ� inst_table�� �����Ѵ�.
 * �Ű� : ���� ��� ���� (���⼭�� inst.data)
 * ��ȯ : �������� = 0 , ���� = -1
 * ���� : inst.data ������ �̸�, ����, op�ڵ�, ���۷��� ������ ������
		����� �����Ͽ� �ۼ��Ǿ��ִٰ� �����Ѵ�.
 * ----------------------------------------------------------------------------------
 */
int init_inst_file(char* inst_file) {
	FILE* file = fopen(inst_file, "r"); // �б� ���� �� ���� ��� ������ ���� ������
	int errno; // �ڵ带 �����ϸ鼭 �������� �ɰ�� 0, �ƴ� ��� -1�� ����Ǹ�, �� ���� �Լ� ����� ���ϵȴ�.

	if (file == NULL)
		errno = -1; // ������ ���� ������ ��� -1�� ����ȴ�.
	else {
		inst* temp; // �� �� �Է¹��� ������ �ӽ÷� ������ ����ü.
		while (feof(file) == 0) {
			temp = (inst*)malloc(sizeof(inst));
			fflush(stdin);
			char c[2]; //format�� �̻��ϰ� ������ ���� �ذ�� �ӽ� ����
			fscanf(file, "%s\t %c\t %x\t %d\n", &temp->str, &c, &temp->op, &temp->ops);
			temp->format = atoi(c);
			inst_table[inst_index++] = temp; // �� ���� ������ ���̺� �����Ѵ�.
		}
		errno = 0; // ������� �ݺ� �����ߴٸ� �������� ���������� ���ϰ� 0�� ����.
	}
	fclose(file);
	return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : ����� �� �ҽ��ڵ带 �о� �ҽ��ڵ� ���̺�(input_data)�� �����ϴ� �Լ�.
 * �Ű� : ������� �ҽ����ϸ� (���⼭�� input.txt)
 * ��ȯ : �������� = 0 , ���� = -1
 * ���� : ���δ����� �����Ѵ�.
 * ----------------------------------------------------------------------------------
 */
int init_input_file(char* input_file) {
	FILE* file = fopen(input_file, "rt"); // �б� ���� �� �ҽ��ڵ� ������ ���� ������
	int errno;

	if (file == NULL) {
		printf("inst_file�� ã�� �� �����ϴ�.\n");
		errno = -1;
	}
	else {
		line_num = 0;
		char* buffer; // �� �� �Է¹��� ������ �ӽ÷� ������ char �迭.
		while (1) {
			buffer = (char*)malloc(sizeof(char) * 100);
			memset(buffer, 0, sizeof(buffer)); // �����Ⱚ ���� ������ ���� 0���� initialize
			fgets(buffer, 100, file); // ���� ������ �ڵ带 �о�´�.
			if (feof(file)) break;
			input_data[line_num] = buffer; // �о�� �� ���� �ҽ��ڵ� ���̺� �����Ѵ�.
			line_num++; // �������� ����Ű�� ���� index�� 1 ����.
		}
		errno = 0; // ������� �ݺ� �����ߴٸ� �������� ���������� ���ϰ� 0�� ����.
	}
	fclose(file);
	return errno; // ���� ��������� �������� ȣ���Լ��� ����.
}

/* ----------------------------------------------------------------------------------
 * ���� : �ҽ� �ڵ带 ���پ� �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�.
		  �н� 1 (assem_pass1)�� ���� ȣ��ȴ�.
 * �Ű� : �Ľ��� ���ϴ� ���ڿ�
 * ��ȯ : �������� = 0 , ���� < 0
 * ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�.
 * ----------------------------------------------------------------------------------
 */
int token_parsing(char* str) {
	if (str[0] == '.')
		return 0;
	char* line_tmp = (char*)malloc(strlen(str) + 2);
	token* tok_temp = (token*)malloc(sizeof(token)); // �� ���� �ҽ��ڵ带 �м��� ��ū���� �ӽ÷� ������ ����.
	char* tok[4] = { '\0', };
	for (int i = 0; i < 4; i++) {
		tok[i] = strsep(&str, "\t\n");
		if (tok[i] == NULL)
			break;
	}

	/* ��ȿ�� �������� Ȯ���ϱ� ���� search_opcode ȣ�� */
	if (search_opcode(tok[1]) < 0) {
		printf("��ȿ���� ���� opcode�Դϴ�.\n");
		return -1;
	}

	tok_temp->label = tok[0];
	tok_temp->operator = tok[1];
	if (tok[3] != NULL)
		strcpy(tok_temp->comment, tok[3]); // �ڸ�Ʈ�� ���̺� ����.

	char* optmp = strtok(tok[2], ","); // ,���� ������ operand�� �ӽ÷� �����ϴ� ����.
	for (int i = 0; i < MAX_OPERAND; i++) {
		if (optmp == NULL) { // �и��� ������ ������� i��° operand �迭�� NULL���ڿ� ����.
			strcpy(tok_temp->operand[i], "\0");
		}
		else { // �и��� ������ ���� ��� i��° operand �迭�� �и��� operand ����.
			strcpy(tok_temp->operand[i], optmp);
		}
		optmp = strtok(NULL, ",");
	}

	token_table[token_line] = tok_temp; // �� ���� �ҽ��ڵ带 ��ūȭ�� ������ ����ü�� ��ū���̺� ����.
	token_line++; // �������� �ҽ��ڵ带 ����ĭ�� �迭�� �����ϱ� ���� �ε��� ����.
	return 0;
}

/* ----------------------------------------------------------------------------------
 * ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�.
 * �Ű� : ��ū ������ ���е� ���ڿ�
 * ��ȯ : �������� = ���� ���̺� �ε��� ��ȣ, ���� = -1, ���þ� = MAX_INST+1
 * ���� : ���þ�� ���� ���̺� �ε����� ����Ǿ����� �ʱ� ������ �Լ� �ȿ� ���ڿ� �迭 ���·� �����.
 * ----------------------------------------------------------------------------------
 */
int search_opcode(char* str) {
	/* ���° ���� ���̺� �ε������� �����ϴ� ����.
	�⺻������ �������� �ش��ϴ� -1�� ����Ǿ��ִ�. */
	int ans = -1;
	char directives[][7] = { "START","END","BYTE","WORD","RESB","RESW","EXTDEF","EXTREF", "LTORG", "EQU", "CSECT" }; // ���þ ������ ���ڿ� �迭�̴�.
	for (int i = 0; i < sizeof(directives) / 7; i++) {
		if (strcmp(str, directives[i]) == 0) {
			ans = MAX_INST + 1; // ���þ� �迭�� �ִٸ� MAX_INST+1 �Ѱ���.
			break;
		}
	}
	// ���þ� �迭�� ���ٸ� ans���� -1 �̹Ƿ� ���� ���̺� �ִ��� �˻��Ѵ�.
	if (ans == -1) {
		int len = strlen(str);
		char* temp_inst = (char*)malloc(len + 1);
		if (str[0] == '+') { // 4���� ��ɾ� ���� + �����ϱ� ���� �ڵ�
			strncpy(temp_inst, str + 1, sizeof(char) * len);
		}
		else strcpy(temp_inst, str);


		for (int i = 0; i < inst_index; i++) {
			if (strcmp(temp_inst, inst_table[i]->str) == 0) {
				ans = i; //���� ��ū string�� inst ���̺� ������ �ε��� ��ȯ ������ ���� �� Ż��.
				break;
			}
		}
	}
	/*
	���� ���̺� �ְų� ���þ� �迭�� �ִٸ� �ش��ϴ� ���� ����Ǿ����� ���̰�,
	 �ƴ� ��� �ʱⰪ���� �������� �ش��ϴ� -1�� �״�� ����Ǿ����� ���̴�.
	 */
	return ans;
}

/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�.
*		�н�1������..
*		1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
*		���̺��� �����Ѵ�.
*		2. locctr�� ���� �ش� ���� �ڵ尡 ��� ��ġ�� �ּҸ� ���� �Ǵ��� ����Ѵ�.
*		3. �� ���� locctr ���� locctr_index �迭�� �����Ѵ�.
*
*
* �Ű� : ����
* ��ȯ : ���� ���� = 0 , ���� = < 0
* ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
		���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�.
*
* -----------------------------------------------------------------------------------
*/
static int assem_pass1(void) {
	token_line = 0;

	/* input_data�� ���ڿ��� ���پ� �Է� �޾Ƽ� ���ڷ� ����ϰ�
	 * token_parsing()���� �Ѱ��־� �ڵ带 ���������ν� token_unit�� ����
	 */
	for (int i = 0; i < line_num; i++) {
		if (token_parsing(input_data[i]) < 0)
			return -1;
	}
	symbol_count = 0; //symtab�� �� �ʱ�ȭ
	locctr = 0;
	int sect_num = 0;
	for (int i = 0; i < token_line; i++) {
		token* temp = token_table[i];
		if (strcmp(temp->label, "\0") != 0) { //�ɺ� ���̺� ����
			if (search_symbol(temp->label, sect_num) != 0) {
				strcpy(sym_table[symbol_count].symbol, temp->label);
				sym_table[symbol_count].addr = locctr;
				sym_table[symbol_count].sector = sect_num;
				symbol_count++;
			}
		}
		if (temp->operand[0][0] == '=') { //literal table�� ����
			int lit_len = strlen(temp->operand[0]);
			char* lit_temp = (char*)calloc(lit_len + 1, sizeof(char));
			strncpy(lit_temp, temp->operand[0] + 3, lit_len - 4);
			if (search_literal(lit_temp) != 0) {
				literal_table[literal_count].literal = (char*)calloc(lit_len + 1, sizeof(char));
				strcpy(literal_table[literal_count].literal, lit_temp);
				literal_table[literal_count].addr = -1;
				if (temp->operand[0][1] == 'C')
					literal_table[literal_count].kind = 1;
				else literal_table[literal_count].kind = 0;
				literal_table[literal_count].sector = sect_num;
				literal_count++;
			}
		}
		if (temp->operator[0] == '+') { //4���� operator���� Ȯ��
			locctr_index[i] = locctr;
			locctr += 4;
		}
		else {
			int indx = search_opcode(temp->operator);
			if (indx < MAX_INST) {
				locctr_index[i] = locctr;
				locctr += inst_table[indx]->format;
			}
			else {
				if (strcmp(temp->operator,"RESW") == 0) {
					locctr_index[i] = locctr;
					int nums = atoi(token_table[i]->operand[0]);
					locctr += nums * 3;
				}
				else if (strcmp(temp->operator,"RESB") == 0) {
					locctr_index[i] = locctr;
					int nums = atoi(token_table[i]->operand[0]);
					locctr += nums;
				}
				else if (strcmp(temp->operator, "EXTDEF") == 0
					|| strcmp(temp->operator, "EXTREF") == 0) {
					locctr_index[i] = -1; //location counter �ʿ����
				}
				else if (strcmp(temp->operator, "LTORG") == 0 ||
					strcmp(temp->operator,"END") == 0) {
					for (int j = 0; j < literal_count; j++) {
						if (literal_table[j].addr == -1) {
							int len;
							if (literal_table[j].kind == 1)
								len = strlen(literal_table[j].literal);
							else len = strlen(literal_table[j].literal) / 2;
							literal_table[j].addr = locctr;
							locctr_index[i] = locctr;
							locctr += len;
						}
					}
				}

				else if (strcmp(temp->operator,"CSECT") == 0) {
					locctr_index[i] = locctr;
				}
				else if (strcmp(temp->operator,"EQU") == 0) {
					if (token_table[i]->operand[0][0] != '*') {
						char* tmp;
						char* tmp_token;
						char operation[2] = { 0, };//������ �����ڸ� �����ϴµ�, strtok ��� ���� ���ڹ迭�� ����.
						int v1, v2;
						tmp = (char*)malloc(strlen(temp->operand[0]) + 1);
						strcpy(tmp, temp->operand[0]);
						for (int j = 0; temp->operand[0][j] != '\0'; j++) {
							if (temp->operand[0][j] == '+' ||
								temp->operand[0][j] == '-') {
								operation[0] = token_table[i]->operand[0][j];
								break;
							}
						}
						tmp_token = strtok(tmp, operation);
						//symtab���� �ּҰ� �������� ���� ����
						for (int k = 0; k < symbol_count; k++) {
							if (strcmp(tmp_token, sym_table[k].symbol) == 0)
								v1 = sym_table[k].addr;
						}
						tmp_token = strtok(NULL, "\0");
						for (int k = 0; k < symbol_count; k++) {
							if (strcmp(tmp_token, sym_table[k].symbol) == 0)
								v2 = sym_table[k].addr;
						}
						if (operation[0] == '+') locctr = v1 + v2;
						else locctr = v1 - v2;
					}
					locctr_index[i] = locctr;

					for (int j = 0; j < symbol_count; j++) {
						if (strcmp(temp->label, sym_table[j].symbol) == 0) {
							sym_table[j].addr = locctr;
						}
					}
				}
				else if (strcmp(temp->operator,"START") == 0)
					locctr_index[i] = locctr;
				else if (strcmp(temp->operator,"*") == 0) {
					locctr_index[i] = locctr;
					if (temp->operand[0][1] == 'C') {
						int ch_num = 0;
						for (int j = 3; temp->operand[0][j] != '\''; j++)
							ch_num++;
						locctr += ch_num;
					}
					else if (temp->operand[0][1] == 'X') {
						int x_num = 0;
						for (int j = 3; temp->operand[0][j] != '\''; j++)
							x_num++;
						locctr += (x_num / 2);
					}
					locctr_index[i + 1] = locctr;
				}
				else if (strcmp(temp->operator,"BYTE") == 0) {
					locctr_index[i] = locctr;
					if (temp->operand[0][0] == 'C') {
						int ch_num = 0;
						for (int j = 2; temp->operand[0][j] != '\''; j++)
							ch_num++;
						locctr += ch_num;
					}
					else if (temp->operand[0][0] == 'X') {
						int x_num = 0;
						for (int j = 2; temp->operand[0][j] != '\''; j++)
							x_num++;
						locctr += (x_num) / 2;
					}
				}
				else if (strcmp(temp->operator, "WORD") == 0) {
					locctr_index[i] = locctr;
					locctr += 3;
				}
			}
		}

		if (i + 1 < token_line && strcmp(token_table[i + 1]->operator,"CSECT") == 0) {
			sect_num++;
			locctr = 0;
		}
	}

	return 0;
}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ ��ɾ� ���� OPCODE�� ��ϵ� ǥ(���� 3��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
		ȭ�鿡 ������ش�.
		���� ���� 3�������� ���̴� �Լ��̹Ƿ� ������ ������Ʈ������ ������ �ʴ´�.
* -----------------------------------------------------------------------------------
*/
void make_opcode_output(char* file_name) {
	if (file_name == NULL) {
		for (int i = 0; i < token_line; i++) {
			token* tmp = token_table[i]; // �� ������ ��ū���� �ҷ��� �ӽ÷� ������ �Լ�.
			printf("%s\t%s\t", tmp->label, tmp->operator); // label�� operator ���.
			for (int j = 0; j < MAX_OPERAND; j++) {
				printf("%s", tmp->operand[j]);
				/* operand�� �� �ִٸ� ','�� ����ϰ� �ƴ� ��� tab��ŭ �� �� for�� Ż��. */
				if (j + 1 < MAX_OPERAND && strcmp(tmp->operand[j + 1], "\0") != 0) {
					printf(",");
				}
				else {
					printf("\t");
					break;
				}
			}
			int index = search_opcode(token_table[i]->operator); // ���� ���̺� �ε������� ����Ǵ� ����.
			if (index == MAX_INST + 1) // ���þ��� ��� inst_table�� ã�� �ʿ䰡 �����Ƿ� tab ��� �� �����ٷ�.
				printf("\t\n");
			else { // �ƴҰ�� op�ڵ� ���.
				if (inst_table[index]->op < 16) // 16���� ���� ��� ���ڸ��ۿ� �ȳ����Ƿ� 0 padding �ʿ����� ���� Ȯ��
					printf("0%X\n", inst_table[index]->op); // 16 �̸��̸� 0 �е� �ʿ���.
				else printf("%X\n", inst_table[index]->op); // 16 �̻��̸� 0 �е� �ʿ� ����.
			}
		}
	}
	else {
		FILE* fp = fopen(file_name, "wt"); // �м������ ������ ������ Write Text ���� ����.
		for (int i = 0; i < token_line; i++) {
			token* tmp = token_table[i]; // �� ������ ��ū���� �ҷ��� �ӽ÷� ������ �Լ�.
			fprintf(fp, "%s\t%s\t", tmp->label, tmp->operator); // label�� operator�� ���Ͽ� ��.
			for (int j = 0; j < MAX_OPERAND; j++) {
				fprintf(fp, "%s", tmp->operand[j]);
				/* operand�� �� �ִٸ� ','�� �����ϰ� �ƴ� ��� tab��ŭ ��� ���� for�� Ż��. */
				if (j + 1 < MAX_OPERAND && strcmp(tmp->operand[j + 1], "\0") != 0) {
					fprintf(fp, ",");
				}
				else {
					fprintf(fp, "\t");
					break;
				}
			}
			int index = search_opcode(token_table[i]->operator); // ���� ���̺� �ε������� ����Ǵ� ����.
			if (index == MAX_INST + 1) {// ���þ��� ��� inst_table�� ã�� �ʿ䰡 �����Ƿ� tab ��� �� �����ٷ�.
				fprintf(fp, "\t\n");
			}
			else {
				if (inst_table[index]->op < 16) // 16���� ���� ��� ���ڸ��ۿ� �ȳ����Ƿ� 0 padding �ʿ����� ���� Ȯ��
					fprintf(fp, "0%X\n", inst_table[index]->op); // 16 �̸��̸� 0 �е� �ʿ���.
				else fprintf(fp, "%X\n", inst_table[index]->op); // 16 �̻��̸� 0 �е� �ʿ� ����.
			}
		}
		fclose(fp);
	}
}
/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ SYMBOL�� �ּҰ��� ����� TABLE�̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/
void make_symtab_output(char* file_name)
{
	FILE* fp = fopen(file_name, "wt");
	for (int i = 0; i < symbol_count; i++) {
		fprintf(fp, "%s\t\t%X\n", sym_table[i].symbol, sym_table[i].addr);
		printf("%s\t\t%X\n", sym_table[i].symbol, sym_table[i].addr);
	}
	fclose(fp);

	printf("\n");
	return;
}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ LITERAL�� �ּҰ��� ����� TABLE�̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/
void make_literaltab_output(char* file_name)
{
	FILE* fp = fopen(file_name, "wt");
	for (int i = 0; i < literal_count; i++) {
		fprintf(fp, "%s\t\t%X\n", literal_table[i].literal, literal_table[i].addr);
		printf("%s\t\t%X\n", literal_table[i].literal, literal_table[i].addr);
	}
	fclose(fp);
	return;
}

/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�.
*		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�.
*		   ������ ���� �۾��� ����Ǿ� ����.
*		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�.
* �Ű� : ����
* ��ȯ : �������� = 0, �����߻� = < 0
* ���� :
* -----------------------------------------------------------------------------------
*/
static int assem_pass2(void)
{
	int sect_num = 0; //���� ���° control section��������.
	for (int i = 0; i < token_line; i++) {
		if (strcmp(token_table[i]->operator,"\0") == 0) continue;
		int idx = search_opcode(token_table[i]->operator); //�ش� operator�� inst_table�� ���° �ٿ� �ִ��� ��ȯ.
		/*operator�� ��ȿ�� ��ɾ���*/
		if (idx >= 0 && idx < inst_index) {
			op_tab[i] |= inst_table[idx]->op;
			switch (inst_table[idx]->format) {
			case 1:
				op_tab[i] = inst_table[idx]->op;
				form_tab[i] = 1;
				break;
			case 2:
				op_tab[i] = inst_table[idx]->op;
				for (int j = 0; j < 2; j++) {
					op_tab[i] <<= 4; //1����Ʈ(4bit)��ŭ shift left
					if (j == 1 && strcmp(token_table[i]->operand[j], "\0") == 0) op_tab[i] |= 0;
					else if (strcmp(token_table[i]->operand[j], "SW") == 0) op_tab[i] |= 9;
					else if (strcmp(token_table[i]->operand[j], "PC") == 0) op_tab[i] |= 8;
					else if (strcmp(token_table[i]->operand[j], "F") == 0) op_tab[i] |= 6;
					else if (strcmp(token_table[i]->operand[j], "T") == 0) op_tab[i] |= 5;
					else if (strcmp(token_table[i]->operand[j], "S") == 0) op_tab[i] |= 4;
					else if (strcmp(token_table[i]->operand[j], "B") == 0) op_tab[i] |= 3;
					else if (strcmp(token_table[i]->operand[j], "L") == 0) op_tab[i] |= 2;
					else if (strcmp(token_table[i]->operand[j], "X") == 0) op_tab[i] |= 1;
					else if (strcmp(token_table[i]->operand[j], "A") == 0) op_tab[i] |= 0;
				}
				form_tab[i] = 2;
				break;
			case 3:
				op_tab[i] = inst_table[idx]->op;
				form_tab[i] = 3;
				op_tab[i] <<= 4; //nixbpe ���� ���� 4��Ʈ��ŭ ������ shift
				if (strcmp(token_table[i]->operand[0], "\0") != 0) {
					/*direct, indirect, ����*/
					if (token_table[i]->operand[0][0] == '@')
						token_table[i]->nixbpe = 2; //�����ּ��϶� ������ 10
					else if (token_table[i]->operand[0][0] == '#')
						token_table[i]->nixbpe = 1; //immediate�϶� ������ 01
					else token_table[i]->nixbpe = 3; //�ƴҰ�� ������ 11
				}
				else token_table[i]->nixbpe = 3;
				token_table[i]->nixbpe <<= 1; //X ��Ʈ �� ���� ����.
				/*X �������� ��� ���� �˻�*/
				for (int j = 0; j < 3 && strcmp(token_table[i]->operand[j], "\0") != 0; j++) {
					if (strcmp(token_table[i]->operand[j], "X") == 0) {
						token_table[i]->nixbpe |= 1;
						break;
					}
				}
				token_table[i]->nixbpe <<= 3; // b,p,e ��Ʈ �� ���� ����.
				/*��ɾ 3�����̰� ���۷��尡 �ִ� ���*/
				if (strcmp(token_table[i]->operand[0], "\0") != 0 && token_table[i]->operator[0] != '+') {
					/*���۷��尡 ��������� ��*/
					if (token_table[i]->operand[0][0] == '@') {
						int target_addr = 0;
						int disp = 0; //displacement
						int pc = 0; //���α׷� ī����
						token_table[i]->nixbpe |= 2; //nixbpe�� p=1, e=0 ����.
						op_tab[i] |= token_table[i]->nixbpe;
						op_tab[i] <<= 12; // ���� 12��Ʈ �� ���� �������.
						pc = locctr_index[i + 1]; //���α׷� ī���Ϳ� ���� �ڵ��� �ּ� ����.
						for (int j = 0; j < symbol_count; j++) {
							if (token_table[i]->operand[0] != NULL && strcmp(sym_table[j].symbol, token_table[i]->operand[0] + 1) == 0)
								target_addr = sym_table[j].addr;
						}
						disp = target_addr - pc;
						disp &= 0xfff;
						op_tab[i] |= disp;
					}
					/*���۷��尡 immediate ����� ��*/
					else if (token_table[i]->operand[0][0] == '#') {
						char* tmp = NULL;
						int immediate;
						int idx = 0;
						op_tab[i] |= token_table[i]->nixbpe;
						op_tab[i] <<= 12;
						tmp = (char*)calloc(strlen(token_table[i]->operand[0]), 1);
						for (int j = 0; j < strlen(token_table[i]->operand[0]) - 1; j++) {
							tmp[idx++] = token_table[i]->operand[0][j + 1];
						}
						tmp[idx] = '\0';
						immediate = strtol(tmp, NULL, 10);
						op_tab[i] |= immediate;
						free(tmp);
					}
					/*���ͷ��� ��*/
					else if (token_table[i]->operand[0][0] == '=') {
						int disp = 0; //displacement
						int pc = 0; //Program Counter
						int target_addr = 0; //Target �ּ�
						token_table[i]->nixbpe |= 2;
						op_tab[i] |= token_table[i]->nixbpe;
						op_tab[i] <<= 12; //12��Ʈ ���� �Ҵ�
						pc = locctr_index[i + 1];
						char* temp;
						int index = 0;
						temp = calloc(strlen(token_table[i]->operand[0]) - 3, 1);
						for (int j = 3; token_table[i]->operand[0][j] != '\''; j++) {
							temp[index++] = token_table[i]->operand[0][j];
						}
						temp[index] = '\0';
						for (int j = 0; j < literal_count; j++) {
							if (strcmp(temp, literal_table[j].literal) == 0) {
								target_addr = literal_table[j].addr;
								break;
							}
						}
						disp = target_addr - pc;
						disp &= 0xfff;
						op_tab[i] |= disp;
					}
					/*Simple Addressing�� ��*/
					else {
						int disp = 0;
						int pc = 0;
						int target_addr = 0;

						token_table[i]->nixbpe |= 2;
						op_tab[i] |= token_table[i]->nixbpe;
						op_tab[i] <<= 12;
						pc = locctr_index[i + 1]; //PC�� ������ ��ɾ� �ּ� ����
						for (int j = 0; j < symbol_count; j++) {
							if (strcmp(token_table[i]->operand[0], "\0") != 0
								&& strcmp(token_table[i]->operand[0], sym_table[j].symbol) == 0 && sect_num == sym_table[j].sector) {
								target_addr = sym_table[j].addr;
								break;
							}
						}
						disp = target_addr - pc;
						disp &= 0xfff;
						op_tab[i] |= disp;
					}
				}
				/* ���۷��尡 ���� ���*/
				else if (strcmp(token_table[i]->operand[0], "\0") == 0) {
					// ���۷��尡 ���°�� p,e = 0,0 �̹Ƿ� nixbpe�� �߰����� �����ʿ� ����.
					op_tab[i] |= token_table[i]->nixbpe;
					op_tab[i] <<= 12;
				}
				/*4������ ��*/
				else if (token_table[i]->operator[0] == '+') {
					int target_addr = 0;
					token_table[i]->nixbpe |= 1; //p,e��Ʈ�� 0,1�� ����.
					op_tab[i] |= token_table[i]->nixbpe;
					op_tab[i] <<= 20; //20��Ʈ�� ���� ����
					for (int j = 0; j < symbol_count; j++) {
						if (strcmp(token_table[i]->operand[0], "\0") == 0
							&& strcmp(token_table[i]->operand, sym_table[j].symbol) == 0) {
							target_addr = sym_table[j].addr;
							break;
						}
					}
					target_addr &= 0xfffff;
					op_tab[i] |= target_addr;
					form_tab[i] = 4;
				}
				break;
			}
		}
		/*operator�� directives �� ��*/
		/*LTORG�� END �� ��*/
		else if (strcmp(token_table[i]->operator,"LTORG") == 0 ||
			strcmp(token_table[i]->operator,"END") == 0) {
			for (int j = 0; j < literal_count; j++) {
				if (literal_table[j].sector == sect_num) {
					int count = strlen(literal_table[j].literal);
					if (literal_table[j].kind == 1) { //���ͷ��� �������� ���
						for (int k = 0; k < count; k++) {
							op_tab[i] |= literal_table[j].literal[k];
							if (k < strlen(literal_table[j].literal) - 1)
								op_tab[i] <<= 8;
						}
						form_tab[i] = count;
					}
					else {
						char* tmp;
						int index = 0;
						tmp = (char*)calloc(count + 1, sizeof(char));
						for (int k = 0; k < count; k++)
							tmp[index++] = literal_table[j].literal[k];
						tmp[index] = '\0';
						op_tab[i] = strtol(tmp, NULL, 16); //16������ �ٲ���
						form_tab[i] = count / 2;
						free(tmp);
					}
				}
			}

		}
		/*operator�� BYTE�� ��*/
		else if (strcmp(token_table[i]->operator,"BYTE") == 0) {
			int cnt = 0;
			int index = 0;
			char* tmp = NULL;

			for (int j = 2; token_table[i]->operand[0][j] != '\''; j++)
				cnt++;
			tmp = (char*)calloc(cnt + 1, sizeof(char));
			for (int j = 2; token_table[i]->operand[0][j] != '\''; j++)
				tmp[index++] = token_table[i]->operand[0][j];
			tmp[index] = '\0';
			op_tab[i] = strtol(tmp, NULL, 16);
			form_tab[i] = cnt / 2;
			free(tmp);
		}
		/*operator�� WORD�� ��*/
		else if (strcmp(token_table[i]->operator,"WORD") == 0) {
			form_tab[i] = 3;
			if (token_table[i]->operand[0][0] >= '0' && token_table[i]->operand[0][0] <= '9') //���ڶ��
				op_tab[i] = atoi(token_table[i]->operand[0]);
			else {
				for (int j = 0; j < token_line; j++) {
					if (strcmp(token_table[i]->operator,"\0") != 0 && strcmp(token_table[i]->operator, "EXTREF") == 0) {
						for (int k = 0; k < 3; k++)
							if (strcmp(token_table[i]->operand[k], "\0") != 0 && strstr(token_table[i]->operand[0], token_table[i]->operand[k]) != NULL)
								continue;
					}
				}
			}
		}
		else if (strcmp(token_table[i]->operator,"CSECT") == 0)
			sect_num++;
	}
}


/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ object code (������Ʈ 1��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
* -----------------------------------------------------------------------------------
*/
void make_objectcode_output(char* file_name)
{
	int start_line = 0; // ���� ������ ���� ��ȣ
	int end_line = 0; // ���� ���� ���� ��ȣ
	int start_addr = 0; //���� ���� �ּ�
	int end_addr = 0; //���� �������� ���α׷� ī����
	int sector = 0; //���� control section ��ȣ
	int is_newline = 1; //���ο� ������ �Ǵ�
	int is_endline = 0; // ��ü ���α׷��� ������ ���������� ���� �˻�.
	int line_char_cnt = 0; //���� �ٿ� ��� ���ڰ� �������� ���.
	int temp_data = 0; //�ּ� ���� �ӽ� ����
	int now = 0; //����ġ

	FILE* fp = fopen(file_name, "w");
	char* tmp;
	char* buffer = (char*)calloc(70, sizeof(char)); // �����͸� ����
	tmp = (char*)calloc(7, sizeof(char));
	while (is_endline == 0) {
		while (strcmp(token_table[now]->label, "/0") != 0 && token_table[now]->label[0] == '.')
			now++;
		/*�� ���α׷�(����)�� ������ �� ���α׷��� �̸�, �����ּ� ���*/
		fprintf(fp, "H%s\t", token_table[now]->label); //���α׷� �̸� ���
		if (strcmp(token_table[now]->operand[0], "\0") != 0)
			start_addr = atoi(token_table[now]->operand[0]);
		fprintf(fp, "%06X", start_addr); // ���α׷� �����ּ�
		/*���α׷�(����) �ϳ��� ������ �ڵ尡 �ִ���, literal�� ��� �ִ��� Ȯ��.*/
		for (int i = now + 1; i < token_line; i++) {
			if (strcmp(token_table[i]->operator,"\0") == 0)
				continue;
			if (strcmp(token_table[i]->operator,"CSECT") == 0)
				break;
			else if (strcmp(token_table[i]->operator,"END") == 0) {
				end_line = i + 1;
				break;
			}
			end_line = i;
		}

		if (end_line == token_line)
			is_endline = 1;
		temp_data = end_line;
		if (end_line != token_line) {
			while (strcmp(token_table[temp_data]->operator,"EQU") == 0)
				temp_data--;

			end_addr = locctr_index[temp_data + 1];//������ ���� PC �ּҰ�.
		}
		else end_addr = locctr_index[temp_data];

		if (sector != 0) {
			if (sector == 1)
				end_addr = 0x2B;
			else end_addr = 0x1C;
		}
		fprintf(fp, "%06X\n", end_addr - start_addr);
		for (int i = now; i <= end_line; i++) {
			if (token_table[i] == NULL)
				continue;
			else {
				/*EXTDEF(�ܺ�����) ó��*/
				if (strcmp(token_table[i]->operator,"EXTDEF") == 0) {
					now = i;
					fputc('D', fp);
					line_char_cnt = 1;
					for (int j = 0; j < 3; j++) {
						if (strcmp(token_table[i]->operand[j], "\0") != 0) {
							int sym_addr = 0;
							/*operand�� �ɺ� ���̺� ���ǵǾ��ִ��� Ȯ��.*/
							for (int k = 0; k < symbol_count; k++) {
								if (strcmp(sym_table[k].symbol, token_table[i]->operand[j]) == 0
									&& sym_table[k].sector == sector)
									sym_addr = sym_table[k].addr;
							}
							line_char_cnt += (strlen(token_table[i]->operand[j]) + 6);
							if (line_char_cnt > 72) { //�� �ٿ� �� �� �ִ� �ִ��� ���̸� ������
								fputs("\nD", fp);
								line_char_cnt = strlen(token_table[i]->operand[j]) + 6;
							}
							fprintf(fp, "%s%06X", token_table[i]->operand[j], sym_addr);
						}
					}
					fputc('\n', fp);
					line_char_cnt = 0;
				}
				/*EXTREF(�ܺ�����) ó��*/
				else if (strcmp(token_table[i]->operator,"EXTREF") == 0) {
					now = i;
					fputc('R', fp);
					line_char_cnt = 1;
					for (int j = 0; j < 3; j++) {
						if (strcmp(token_table[i]->operand[j], "\0") != 0) {
							line_char_cnt += strlen(token_table[i]->operand[j]) + 1;
							if (line_char_cnt > 72) { //�� �ٿ� �� �� �ִ� �ִ��� ���̸� ������
								fputs("\nR", fp);
								line_char_cnt = strlen(token_table[i]->operand[j]) + 1;
							}
							fprintf(fp, "%s", token_table[i]->operand[j]);
						}
					}
					fputc('\n', fp);
					line_char_cnt = 0;
				}
			}
		}
		now++;
		start_line = now;
		int tmp_end = 0;
		while (now <= end_line) {
			if (is_newline) {
				sprintf(buffer, "T%06X", (locctr_index[now]>locctr_index[now-1]?locctr_index[now]:locctr_index[now-1]));
				line_char_cnt = 7;
				strcat(buffer, "00"); //������ ���� ��Ÿ���� ��Ʈ�� 00���� �д�
				line_char_cnt += 2;
				is_newline = 0;
			}
			switch (form_tab[now]) {
			case 1:
				line_char_cnt += 2;
				/*���� ��ġ�� ���� ���� ���� 69 �Ѿ���� ���� Ȯ��*/
				if (line_char_cnt > 69) { 
					line_char_cnt -= 2;
					sprintf(tmp, "%02X", (strlen(buffer) - 9) / 2);
					//�� ���� ���̸� buffer�� ����
					buffer[7] = tmp[0];
					buffer[8] = tmp[1];
					fputs(buffer, fp); //buffer�� ������ ���Ͽ� ����
					fputc('\n', fp);
					line_char_cnt = 0;
					sprintf(buffer, "T%06X", locctr_index[now]); //���ο� ���� ���� ���� ���� buffer�� �ʱ�ȭ
					line_char_cnt += 7;
					strcat(buffer, "00");
					strcat(buffer, tmp);
					line_char_cnt += 2;
				}
				/*�� control section(=sector)�� ������ ���*/
				else if (form_tab[now + 1] <= 0 && token_table[now + 1] != NULL
					&& strcmp(token_table[now + 1]->operator,"END") != 0) {
					sprintf(tmp, "%02X", op_tab[now++]);
					strcat(buffer, tmp);
					sprintf(tmp, "%02X", (strlen(buffer) - 9) / 2);
					buffer[7] = tmp[0];
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
					line_char_cnt = 0;
					is_newline = 1;
				}
				/*�Ѿ�� ������ buffer�� ����*/
				else {
					sprintf(tmp, "%02X", op_tab[now++]);
					strcat(buffer, tmp);
				}
				break;
			case 2:
				line_char_cnt += 4;
				if (line_char_cnt > 69) {
					line_char_cnt -= 4;
					sprintf(tmp, "%02X", (strlen(buffer) - 9) / 2) + 1;
					buffer[7] = tmp[0];
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
					line_char_cnt = 0;
					sprintf(buffer, "T%06X", locctr_index[now]);
					line_char_cnt += 7;
					strcat(buffer, "00");
					line_char_cnt += 2;
					sprintf(tmp, "%04X", op_tab[now++]);
					strcat(buffer, tmp);
					line_char_cnt += 4;
				}
				else if (form_tab[now + 1] <= 0 && token_table[now + 1] != NULL
					&& strcmp(token_table[now + 1]->operator,"END") != 0) {
					sprintf(tmp, "%02X", op_tab[now++]);
					strcat(buffer, tmp);
					sprintf(tmp, "%02X", (strlen(buffer) - 9) / 2);
					buffer[7] = tmp[0];
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
					line_char_cnt = 0;
					is_newline = 1;
				}
				else {
					sprintf(tmp, "%04X", op_tab[now++]);
					strcat(buffer, tmp);
				}
				break;
			case 3:
				line_char_cnt += 6;
				if (line_char_cnt > 69) {
					line_char_cnt -= 6;
					sprintf(tmp, "%02X", (strlen(buffer) - 8) / 2);
					buffer[7] = tmp[0];
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
					line_char_cnt = 0;
					sprintf(buffer, "T%06X", locctr_index[now]);
					line_char_cnt += 7;
					strcat(buffer, "00");
					line_char_cnt += 2;
					sprintf(tmp, "%06X", op_tab[now++]);
					strcat(buffer, tmp);
					line_char_cnt += 6;
				}
				else if (form_tab[now + 1] <= 0 && token_table[now + 1] != NULL
					&& strcmp(token_table[now + 1]->operator,"END") != 0) {
					sprintf(tmp, "%06X", op_tab[now++]);
					strcat(buffer, tmp);
					sprintf(tmp, "%02X", (strlen(buffer) - 9) / 2);
					buffer[7] = tmp[0];
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
					line_char_cnt = 0;
					is_newline = 1;
				}
				else {
					sprintf(tmp, "%06X", op_tab[now++]);
					strcat(buffer, tmp);
				}
				break;
			case 4:
				line_char_cnt += 8;
				if (line_char_cnt > 69) {
					line_char_cnt -= 8;
					sprintf(tmp, "%02X", (strlen(buffer) - 9) / 2);
					buffer[7] = tmp[0];
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
					line_char_cnt = 0;
					sprintf(buffer, "T%06X", locctr_index[now]);
					line_char_cnt += 7;
					strcat(buffer, "00");
					line_char_cnt += 2;
					sprintf(tmp, "%08X", op_tab[now++]);
					strcat(buffer, tmp);
					line_char_cnt += 8;
				}
				else if (form_tab[now + 1] <= 0 && token_table[now + 1] != NULL
					&& strcmp(token_table[now + 1]->operator,"END") != 0) {
					sprintf(tmp, "%08X", op_tab[now++]);
					strcat(buffer, tmp);
					sprintf(tmp, "%02X", (strlen(buffer) - 9) / 2);
					buffer[7] = tmp[0];
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
					line_char_cnt = 0;
					is_newline = 1;
				}
				else {
					sprintf(tmp, "%08X", op_tab[now++]);
					strcat(buffer, tmp);
				}
				break;
			default:
				if (token_table[now] != NULL && strcmp(token_table[now]->operator,"END") == 0)
					tmp_end = now;
				else if (strcmp(token_table[now - 2]->operator,"\0") != 0 && strcmp(token_table[tmp_end]->operator,"END") == 0
					&& token_table[now] != NULL) {
					sprintf(tmp, "%02X", (strlen(buffer) - 9) / 2);
					buffer[7] = tmp[0];
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
				}
				else if (token_table[now] == NULL) {
					sprintf(tmp, "%02X", (strlen(buffer) - 9) / 2);
					buffer[7] = tmp[0];
					buffer[8] = tmp[1];
					fputs(buffer, fp);
					fputc('\n', fp);
				}
				else {
					is_newline = 1;
					line_char_cnt = 0;
				}
				now++;
			}
		}
		now--;
		for (int i = start_line - 1; i <= end_line; i++) {
			/*�ܺ�����(EXTREF) ó��*/
			if (strcmp(token_table[i]->operator,"EXTREF") == 0) {
				char* index = NULL;
				for (int j = 0; j < 2; j++) {
					if (strcmp(token_table[i]->operand[j], "\0") != 0) {
						for (int k = start_line; k <= end_line; k++) {
							if (token_table[k] != NULL) {
								if ((index = strstr(token_table[k]->operand[0], token_table[i]->operand[j])) != NULL) {
									if (search_opcode(token_table[k]->operator) <= MAX_INST)
										fprintf(fp, "M%06X05+%s\n", locctr_index[k] + 1, token_table[i]->operand[j]);
									else {
										if ((index - 1)[0] == '-')
											fprintf(fp, "M%06X06-%s\n", locctr_index[k] + 1, token_table[i]->operand[j]);
										else
											fprintf(fp, "M%06X06+%s\n", locctr_index[k] + 1, token_table[i]->operand[j]);
									}
								}
							}
						}
					}
				}
				break;
			}
		}
		if (sector == 0)
			fprintf(fp, "E%06X\n\n", locctr_index[start_line]);
		else fputs("E\n\n", fp);
		sector++;
		now++;
	}
	fclose(fp);
}


/* ----------------------------------------------------------------------------------
* ���� : strcmp�� ���� ������ ���� ������ �ذ��ϱ� ���� �߰������� ������ �Լ��̴�.
* �Ű� : ������ string, ������.
* ��ȯ : ���е� ��ū �迭
* ���� :
* -----------------------------------------------------------------------------------
*/
char* strsep(char** stringp, const char* delim) {
	char* start = *stringp;
	char* p;

	p = (start != NULL) ? strpbrk(start, delim) : NULL;

	if (p == NULL) {
		*stringp = NULL;
	}
	else {
		*p = '\0';
		*stringp = p + 1;
	}

	return start;
}

int search_literal(char* str) {
	for (int i = 0; i < literal_count; i++) {
		if (strcmp(literal_table[i].literal, str) == 0)
			return 0;
	}
	return -1;
}

int search_symbol(char* str, int sector_num) {
	for (int i = 0; i < symbol_count; i++) {
		if (strcmp(sym_table[i].symbol, str) == 0) {
			if (sector_num == sym_table[i].sector)
				return 0;
		}
	}
	return -1;
}