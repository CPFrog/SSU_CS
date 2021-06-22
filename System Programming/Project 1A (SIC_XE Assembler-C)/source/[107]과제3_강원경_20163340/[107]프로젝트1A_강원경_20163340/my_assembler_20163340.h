/* 
 * my_assembler �Լ��� ���� ���� ���� �� ��ũ�θ� ��� �ִ� ��� �����̴�. 
 * 
 */
#define MAX_INST 256
#define MAX_LINES 5000
#define MAX_OPERAND 3

 /*
  * instruction ��� ���Ϸ� ���� ������ �޾ƿͼ� �����ϴ� ����ü �����̴�.
  * ������ ������ instruction set�� ��Ŀ� ���� ���� �����ϵ�
  * ���� ���� �ϳ��� instruction�� �����Ѵ�.
  */
struct inst_unit
{
	char str[10];
	unsigned char op;
	int format;
	int ops;
};
// instruction�� ������ ���� ����ü�� �����ϴ� ���̺� ����
typedef struct inst_unit inst;
inst *inst_table[MAX_INST];
int inst_index;

/*
 * ����� �� �ҽ��ڵ带 �Է¹޴� ���̺��̴�. ���� ������ ������ �� �ִ�.
 */
char *input_data[MAX_LINES];
static int line_num;

/*
 * ����� �� �ҽ��ڵ带 ��ū������ �����ϱ� ���� ����ü �����̴�.
 * operator�� renaming�� ����Ѵ�.
 * nixbpe�� 8bit �� ���� 6���� bit�� �̿��Ͽ� n,i,x,b,p,e�� ǥ���Ѵ�.
 */
struct token_unit
{
	char* label;
	char* operator;
	char operand[MAX_OPERAND][20];
	char comment[100];
	char nixbpe;
};

typedef struct token_unit token;
token* token_table[MAX_LINES];
static int token_line;

char *operand_set[MAX_LINES];

/*
 * �ɺ��� �����ϴ� ����ü�̴�.
 * �ɺ� ���̺��� �ɺ� �̸�, �ɺ��� ��ġ�� �����ȴ�.
 */
struct symbol_unit
{
	char symbol[10];
	int addr;
	int sector; // ���° control section�� �ִ��� ����
};

/*
* ���ͷ��� �����ϴ� ����ü�̴�.
* ���ͷ� ���̺��� ���ͷ��� �̸�, ���ͷ��� ��ġ�� �����ȴ�.
*/
struct literal_unit
{
	char* literal;
	int addr;
	int kind; //Character=1, X=0
	int sector; // ���° control section�� �ִ��� ����
};

typedef struct symbol_unit symbol;
symbol sym_table[MAX_LINES];
int symbol_count; //symtab�� ���� ����

typedef struct literal_unit literal;
literal literal_table[MAX_LINES];
int literal_count;

static int locctr;
//--------------

static char *input_file;
static char *output_file;
int init_my_assembler(void);
int init_inst_file(char *inst_file);
int init_input_file(char *input_file);
int token_parsing(char *str);
int search_opcode(char *str);
static int assem_pass1(void);
void make_opcode_output(char *file_name);

void make_symtab_output(char *file_name);
void make_literaltab_output(char* file_name);
static int assem_pass2(void);
void make_objectcode_output(char *file_name);

/* �߰� ���� ����*/
int locctr_index[MAX_LINES]; //location counter ��ġ ����� 
int op_tab[MAX_LINES]; // �� ���� ���� �ڵ带 ������ ����
int form_tab[MAX_LINES]; // op_tab�� �� �ٿ��� ��� ��Ʈ�� ������ �ϴ��� �Ǵ� ������ �Ǵ� ���̺�
int op_line; //op_tab�� ���� �����Ͱ� ����ִ� ���� ����.

/*�߰� ���� �Լ�*/
char* strsep(char** stringp, const char* delim); //strtok�� ���� ������ ���� issue�� �ذ��ϱ� ���� ������ �Լ�.
int search_literal(char* str); //���ͷ� ���̺� �ִ��� Ȯ���ϴ� �Լ�
int search_symbol(char* str, int sector_num); //symbol table�� �ִ��� Ȯ���ϴ� �Լ�