#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "person.h"
#define HEAD_RECORD_SIZE 16
//필요한 경우 헤더 파일과 함수를 추가할 수 있음

// 과제 설명서대로 구현하는 방식은 각자 다를 수 있지만 약간의 제약을 둡니다.
// 레코드 파일이 페이지 단위로 저장 관리되기 때문에 사용자 프로그램에서 레코드 파일로부터 데이터를 읽고 쓸 때도
// 페이지 단위를 사용합니다. 따라서 아래의 두 함수가 필요합니다.
// 1. readPage(): 주어진 페이지 번호의 페이지 데이터를 프로그램 상으로 읽어와서 pagebuf에 저장한다
// 2. writePage(): 프로그램 상의 pagebuf의 데이터를 주어진 페이지 번호에 저장한다
// 레코드 파일에서 기존의 레코드를 읽거나 새로운 레코드를 쓰거나 삭제 레코드를 수정할 때나
// 위의 readPage() 함수를 호출하여 pagebuf에 저장한 후, 여기에 필요에 따라서 새로운 레코드를 저장하거나
// 삭제 레코드 관리를 위한 메타데이터를 저장합니다. 그리고 난 후 writePage() 함수를 호출하여 수정된 pagebuf를
// 레코드 파일에 저장합니다. 반드시 페이지 단위로 읽거나 써야 합니다.
//
// 주의: 데이터 페이지로부터 레코드(삭제 레코드 포함)를 읽거나 쓸 때 페이지 단위로 I/O를 처리해야 하지만,
// 헤더 레코드의 메타데이터를 저장하거나 수정하는 경우 페이지 단위로 처리하지 않고 직접 레코드 파일을 접근해서 처리한다.

struct header {
	int total_page_num;
	int total_record_num;
	int recent_delete_page;
	int recent_delete_record;
} headers;

typedef struct _Pointer {
	int page_num;
	int record_num;
} Pointer;

void readPage(FILE *fp, char *pagebuf, int pagenum);
void writePage(FILE *fp, const char *pagebuf, int pagenum);
void pack(char *recordbuf, const Person *p);
void unpack(const char *recordbuf, Person *p);
void add(FILE *fp, const Person *p);
void delete (FILE *fp, const char *id);
void addNewPage(FILE *fp);
void appendRecord(FILE *fp, char *pagebuf, char *recordbuf);

//
// 페이지 번호에 해당하는 페이지를 주어진 페이지 버퍼에 읽어서 저장한다. 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
void readPage(FILE *fp, char *pagebuf, int pagenum) {
	fseek(fp, pagenum * PAGE_SIZE + HEAD_RECORD_SIZE, SEEK_SET);
	fread(pagebuf, PAGE_SIZE, 1, fp);
}

//
// 페이지 버퍼의 데이터를 주어진 페이지 번호에 해당하는 레코드 파일의 위치에 저장한다.
// 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
void writePage(FILE *fp, const char *pagebuf, int pagenum) {
	fseek(fp, pagenum * PAGE_SIZE + HEAD_RECORD_SIZE, SEEK_SET);
	fwrite(pagebuf, PAGE_SIZE, 1, fp);
}

//
// 새로운 레코드를 저장할 때 터미널로부터 입력받은 정보를 Person 구조체에 먼저 저장하고, pack() 함수를 사용하여
// 레코드 파일에 저장할 레코드 형태를 recordbuf에 만든다.
//
void pack(char *recordbuf, const Person *p) {
	sprintf(recordbuf, "%s#%s#%s#%s#%s#%s#", p->id, p->name, p->age, p->addr, p->phone, p->email);
}

//
// 아래의 unpack() 함수는 recordbuf에 저장되어 있는 레코드를 구조체로 변환할 때 사용한다.
//
void unpack(const char *recordbuf, Person *p) {
	char *buf = (char *)malloc(strlen(recordbuf) + 1);
	memcpy(buf, recordbuf, strlen(recordbuf));
	char *temp = strsep(&buf, "#");
	strcpy(p->id, temp);
	temp = strsep(&buf, "#");
	strcpy(p->name, temp);
	temp = strsep(&buf, "#");
	strcpy(p->age, temp);
	temp = strsep(&buf, "#");
	strcpy(p->addr, temp);
	temp = strsep(&buf, "#");
	strcpy(p->phone, temp);
	temp = strsep(&buf, "#");
	strcpy(p->email, temp);

}

//
// 새로운 레코드를 저장하는 기능을 수행하며, 터미널로부터 입력받은 필드값들을 구조체에 저장한 후 아래 함수를 호출한다.
//
void add(FILE *fp, const Person *p)
{
	// 레코드 버퍼 생성
	char *recordbuf = (char *)malloc(MAX_RECORD_SIZE + 1);
	pack(recordbuf, p);

	// 헤더 레코드 정보 불러오기.
	fseek(fp, 0, SEEK_SET);
	fread(&headers, sizeof(headers), 1, fp);

	Pointer cur = {.page_num = headers.recent_delete_page, .record_num = headers.recent_delete_record};

	//페이지 버퍼 생성
	char *pagebuf = (char *)malloc(PAGE_SIZE + 1);

	// 삭제한 레코드가 없을 때 :: 단순히 레코드 추가
	if (cur.page_num == -1 && cur.record_num == -1)
	{
		appendRecord(fp, pagebuf, recordbuf);
	}
	else
	{ // 만약 삭제 레코드가 있다면

		Pointer prev;	  // 현재 레코드보다 오래전에 삭제된거
		Pointer recent;	  //현재 레코드보다 최근에 삭제된거
		int hasfound = 0; // 삭제 레코드들 중에서 넣을 공간을 찾았다면 1, 못찾았다면 0

		//이전 데이터 영역을 역추적하면서 여유 공간 판단
		while (1)
		{
			// 현재 삭제 데이터의 오프셋과 길이 가져옴.
			int cur_offset, cur_length;

			readPage(fp, pagebuf, cur.page_num);
			memcpy(&cur_offset, pagebuf + 4 + (8 * cur.record_num), sizeof(int)); //레코드 번호는 0번부터 저장되므로 -1 할 필요 없음.
			memcpy(&cur_length, pagebuf + 8 * (cur.record_num + 1), sizeof(int));

			// 이전 삭제 레코드에 대한 페이지 위치, 레코드 번호를 prev 구조체로 가져옴
			memcpy(&prev, pagebuf + HEADER_AREA_SIZE + cur_offset + 1, sizeof(Pointer)); //+1 이유 = "*" 문자 때문


			// 현재 위치의 삭제 레코드에 새 레코드를 넣을 수 있다면
			if (cur_length >= strlen(recordbuf))
			{
				// 넣을 공간을 찾았으므로 hasfound=1
				hasfound = 1;

				// 1. 가장 최근 삭제 레코드인 경우
				if (cur.page_num == headers.recent_delete_page && cur.record_num == headers.recent_delete_record)
				{
					// 헤더 레코드의 최근 삭제 레코드 정보를 이전 레코드로 연결
					headers.recent_delete_page = prev.page_num;
					headers.recent_delete_record = prev.record_num;

					// 갱신된 헤더 레코드의 정보를 파일에도 반영
					fseek(fp, 0, SEEK_SET);
					fwrite(&headers, sizeof(headers), 1, fp);

					// 페이지 버퍼에 새 레코드 내용 저장
					memcpy(pagebuf + HEADER_AREA_SIZE + cur_offset, recordbuf, strlen(recordbuf));

					// 페이지 버퍼의 내용을 파일에 저장
					writePage(fp, pagebuf, cur.page_num);

					break;
				}
				else
				{ // 중간 또는 마지막 삭제 레코드인 경우.
					memcpy(pagebuf + HEADER_AREA_SIZE + cur_offset, recordbuf, strlen(recordbuf));
					writePage(fp, pagebuf, cur.page_num);

					// 현재 레코드보다 비교적 더 최근에 삭제된 레코드의 페이지 불러옴
					readPage(fp, pagebuf, recent.page_num);

					int recent_offset; // 비교적 최근 삭제된 레코드의 데이터 영역 내 오프셋 위치

					// 최근 삭제 레코드의 오프셋 가져옴
					memcpy(&recent_offset, pagebuf + 4 + (8 * recent.record_num), sizeof(int)); //레코드 번호는 0번부터 저장되므로 -1 할 필요 없음.

					// 페이지 버퍼의 최근 삭제 레코드 offset "*" 이후 8바이트에 이전 삭제 레코드의 위치로 내용 갱신.
					memcpy(pagebuf + HEADER_AREA_SIZE + recent_offset + 1, &prev, sizeof(Pointer));

					// 최근 삭제 레코드의 페이지 갱신 내용을 파일에도 갱신.
					writePage(fp, pagebuf, recent.page_num);

					// 새로운 레코드가 기록될 현재 삭제 레코드의 페이지 가져옴.
					readPage(fp, pagebuf, cur.page_num);
				}
			}

			//TODO: 계속 순회하는 로직의 나머지 구현
			if (prev.page_num == -1 && prev.record_num == -1)
				break;
			recent = cur;
			cur = prev;
		}

		// TODO : 삭제레코드는 있지만, 여유 공간이 없는 경우 레코드를 append 시킴.
		if (hasfound == 0){
			appendRecord(fp, pagebuf, recordbuf);
		}

		// TODO: if 삭제 레코드 영역에 들어갈 수 있다면 몇번째 삭제지점인지에 대한 판단 필요
		// TODO : 1. 가장 최근 삭제 레코드인 경우 헤더 영역에 자기 자신이 가리키고 있던 위치 갱신.
		// TODO : 2. 중간 순서에 있는 레코드에 저장 시 A->B->C에서 A->C로의 과정 필요
		// TODO : 3. 마지막 삭제 레코드인 경우 이전 레코드를 -1 -1로 해주는 과정 필요. (2번 조건에 if문 달면 될 듯.)
		// TODO : else if 삭제 레코드에 들어갈 수 없다면 데이터를 append.
	}
	free(pagebuf);
	free(recordbuf);
}

//
// 주민번호와 일치하는 레코드를 찾아서 삭제하는 기능을 수행한다.
//
void delete (FILE *fp, const char *id) {
	for (int i = 0; i < headers.total_page_num; i++)
	{
		char *pagebuf = (char *)malloc(PAGE_SIZE+1); // 페이지 버퍼 공간 할당
		readPage(fp, pagebuf, i);											 // 페이지 버퍼에 페이지 읽어옴.
		int record_num;														 // 그 페이지에 저장된 레코드의 갯수 저장하는 변수 (#record)
		memcpy(&record_num, pagebuf, sizeof(int));							 // 그 페이지에 저장된 레코드의 갯수 불러옴
		for (int j = 0; j < record_num; j++)
		{
			int offset, length;										 // 각 레코드의 오프셋
			memcpy(&offset, pagebuf + 4 + 8 * j , sizeof(int)); // #record 4byte + 레코드당 메타데이터 8byte
			memcpy(&length, pagebuf + 8 * (j+1), sizeof(int));

			Person temp_person;
			char *recordbuf = (char *)malloc(MAX_RECORD_SIZE);
			memcpy(recordbuf, pagebuf + HEADER_AREA_SIZE + offset, length);

			unpack(recordbuf, &temp_person);

			if (strcmp(temp_person.id, id) == 0) // 삭제할 레코드의 주민번호와 현 위치 레코드의 주민번호가 일치하다면
			{
				char* delete_marks = (char*)malloc(10);										  // delete에 대한 내용이 담길 10바이트 buffer ('*' 1byte, 페이지 번호 4byte, 레코드 번호 4byte, null 문자 1byte)
				strcpy(delete_marks,"*");
				int temp_pg=headers.recent_delete_page, temp_rcd=headers.recent_delete_record;
				memcpy(delete_marks + 1, &temp_pg, sizeof(int));	  //delete_marks에 이전 삭제 레코드의 페이지 번호 기록
				memcpy(delete_marks + 5, &temp_rcd, sizeof(int)); //delete_marks에 이전 삭제 레코드의 페이지 내 레코드 번호 기록

				//헤더에 지금 삭제하는 레코드에 대한 정보 반영
				headers.recent_delete_page = i;	  // 페이지 번호
				headers.recent_delete_record = j; // 레코드 번호

				//헤더 레코드에 반영한 정보를 파일에도 반영
				fseek(fp, 0, SEEK_SET);
				fwrite(&headers, sizeof(headers), 1, fp);

				// pagebuf의 삭제 레코드의 위치에 delete_marks 기록
				memcpy(pagebuf + HEADER_AREA_SIZE + offset, delete_marks, 9);

				// 갱신된 pagebuf를 파일에 기록.
				writePage(fp, pagebuf, i);
				return;
			}
		}
	}
}

/*-----추가 정의한 함수.------*/
//새로운 페이지를 생성하여 레코드 파일의 맨 뒤에 append 한다.
void addNewPage(FILE *fp)
{
	// 헤더 레코드 불러옴
	fseek(fp, 0, SEEK_SET);
	fread(&headers, sizeof(headers), 1, fp);

	// 새로운 페이지의 내용을 0xff로 채움
	char *pagebuf = (char *)calloc(PAGE_SIZE + 1, sizeof(char));
	memset(pagebuf, 0xff, PAGE_SIZE);

	// 새로운 페이지의 레코드 갯수를 0으로 세팅
	int record_num = 0;
	memcpy(pagebuf, &record_num, sizeof(int));

	// 파일에 새로운 페이지를 이어붙임.
	writePage(fp, pagebuf, headers.total_page_num);

	// 헤더 레코드에 전체 페이지 갯수를 1 증가시킴
	headers.total_page_num += 1;

	// 페이지 갯수가 갱신된 헤더 레코드를 파일에도 반영
	fseek(fp, 0, SEEK_SET);
	fwrite(&headers, sizeof(headers), 1, fp);

	free(pagebuf);
}

// 레코드 파일에 데이터를 append 한다.
void appendRecord(FILE *fp, char *pagebuf, char *recordbuf)
{
	readPage(fp, pagebuf, headers.total_page_num - 1);
	int record_num;
	memcpy(&record_num, pagebuf, sizeof(int));
	int offset = 0, length = 0;
	int record_len = strlen(recordbuf);

	// append 하는 페이지의 레코드의 갯수(record_num)가 0이라면 단순히 저장
	if (record_num == 0)
	{
		record_num = 1;

		memcpy(pagebuf, &record_num, sizeof(int));					// 페이지의 레코드 갯수 1로 갱신
		memcpy(pagebuf + 4, &offset, sizeof(int));					// 오프셋 정보 저장
		memcpy(pagebuf + 8, &record_len, sizeof(int));				// 길이 정보 저장
		memcpy(pagebuf + HEADER_AREA_SIZE, recordbuf, record_len); // 내용 저장

		writePage(fp, pagebuf, headers.total_page_num - 1);
	}
	// record_num이 0이 아니라면 페이지 크기 초과하는지에 대한 판단 필요
	else
	{
		memcpy(&offset, pagebuf + (8 * (record_num - 1) + 4), sizeof(int)); // 페이지 내 마지막 레코드의 오프셋 불러옴
		memcpy(&length, pagebuf + (8 * record_num), sizeof(int));			// 페이지 내 마지막 레코드의 길이 불러옴

		// 만약 새로운 레코드를 추가할 때 데이터 영역 크기 또는 헤더 영역 크기를 초과한다면 새로운 페이지 생성 후 그곳에 저장.
		if (offset + length + record_len > DATA_AREA_SIZE || 4 + (record_num + 1) * 8 > HEADER_AREA_SIZE)
		{
			addNewPage(fp);
			readPage(fp, pagebuf, headers.total_page_num - 1); //addNewPage 과정에서 전역변수 headers의 페이지 갯수가 갱신되므로 파일에서 헤더레코드를 다시 불러올 필요 없음.
			offset = 0;
			record_num = 1;

			memcpy(pagebuf, &record_num, sizeof(int));						  // 페이지의 레코드 갯수 1 증가
			memcpy(pagebuf + 4, &offset, sizeof(int));						  // 오프셋 정보 저장
			memcpy(pagebuf + 8, &record_len, sizeof(int));					  // 길이 정보 저장
			memcpy(pagebuf + HEADER_AREA_SIZE, recordbuf, strlen(recordbuf)); // 내용 저장 !헷갈림! strlen()?? sizeof()??

			writePage(fp, pagebuf, headers.total_page_num - 1);
		}
		// 아니라면 해당 페이지에 이어붙이면 됨.
		else
		{
			offset += length;
			record_num += 1;

			memcpy(pagebuf, &record_num, sizeof(int));
			memcpy(pagebuf + (4 + 8 * (record_num - 1)), &offset, sizeof(int));
			memcpy(pagebuf + 8 * record_num, &record_len, sizeof(int));
			memcpy(pagebuf + HEADER_AREA_SIZE + offset, recordbuf, strlen(recordbuf));

			writePage(fp, pagebuf, headers.total_page_num - 1);
		}
	}
	// TODO : 헤더 레코드에 레코드 갯수 +1 필요
	headers.total_record_num += 1;
	fseek(fp, 0, SEEK_SET);
	fwrite(&headers, sizeof(headers), 1, fp);
}
/*----- 추가 정의한 함수 끝 -----*/

int main(int argc, char *argv[])
{
	FILE *fp; // 레코드 파일의 파일 포인터

	fp = fopen(argv[2], "r+b");
	if (fp == NULL)
	{
		//fclose(fp);
		fp = fopen(argv[2], "w+b");
		char *temp = (char *)calloc(HEAD_RECORD_SIZE + 1, sizeof(char));
		char *pagebuf = (char *)calloc(PAGE_SIZE + 1, sizeof(char));
		headers.total_page_num = 1;
		headers.total_record_num = 0;
		headers.recent_delete_page = -1;
		headers.recent_delete_record = -1;
		memcpy(temp, &headers, sizeof(headers));
		fwrite(temp, sizeof(headers), 1, fp);
		memset(pagebuf, 0xff, PAGE_SIZE);
		memset(pagebuf, 0, sizeof(int));
		writePage(fp, pagebuf, 0);
		free(temp);
		free(pagebuf);
		fclose(fp);
		fp = fopen(argv[2], "r+b");
	}
	fseek(fp, 0, SEEK_SET);
	fread(&headers, sizeof(headers), 1, fp);

	if (strcmp(argv[1], "a") == 0)
	{
		Person p;
		strcpy(p.id, argv[3]);
		strcpy(p.name, argv[4]);
		strcpy(p.age, argv[5]);
		strcpy(p.addr, argv[6]);
		strcpy(p.phone, argv[7]);
		strcpy(p.email, argv[8]);
		add(fp, &p);
	}
	else
	{
		delete(fp, argv[3]);
	}

	return 0;
}
