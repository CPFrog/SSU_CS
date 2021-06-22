// 주의사항
// 1. blockmap.h에 정의되어 있는 상수 변수를 우선적으로 사용해야 함
// 2. blockmap.h에 정의되어 있지 않을 경우 본인이 이 파일에서 만들어서 사용하면 됨
// 3. 필요한 data structure가 필요하면 이 파일에서 정의해서 쓰기 바람(blockmap.h에 추가하면 안됨)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "blockmap.h"
// 필요한 경우 헤더 파일을 추가하시오.

//
// flash memory를 처음 사용할 때 필요한 초기화 작업, 예를 들면 address mapping table에 대한
// 초기화 등의 작업을 수행한다. 따라서, 첫 번째 ftl_write() 또는 ftl_read()가 호출되기 전에
// file system에 의해 반드시 먼저 호출이 되어야 한다.
//

extern int dd_read(int ppn, char* pagebuf);
extern int dd_write(int ppn, char* pagebuf);
extern int dd_erase(int pbn);

int lbn_table[DATABLKS_PER_DEVICE]; //index: lbn, value: pbn
int pbn_table[DATABLKS_PER_DEVICE]; //해당 pbn의 영역이 비어있는지 여부 저장
int free_pbn = DATABLKS_PER_DEVICE;

void ftl_open() {
	memset(lbn_table, -1, sizeof(lbn_table));
	memset(pbn_table, -1, sizeof(pbn_table));
	char* pagebuf = (char*)malloc(PAGE_SIZE + 1);
	for (int pbn = 0; pbn < DATABLKS_PER_DEVICE; pbn++) {
		/*ftl에서는 파일을 직접 읽을 수 없으므로 (개념적으로도 그렇고, 구현상으로도 그럼.)
		* flash device driver에서 page 단위로 읽어와야함*/

		int ppn=pbn*PAGES_PER_BLOCK;
		memset(pagebuf,'\0',sizeof(pagebuf));
		dd_read(ppn, pagebuf);	//dd_read 함수 통해 각 블록 첫 페이지를 읽어옴.
		int lbn = 0;
		/*각 블록의 첫 페이지 spare 영역의 4byte 읽어 lbn 추출*/
		memcpy(&lbn, pagebuf + SECTOR_SIZE, sizeof(lbn));
		/*lbn이 -1이 아닐경우 lbn과 매칭되는 pbn이 있다는 뜻이므로
		* 테이블에 lbn과 매칭되는 pbn 번호 넣음*/
		if (lbn != 0xffffffff) {
			lbn_table[lbn] = pbn;
			pbn_table[pbn] = lbn;
		}
		else {
			/*free_block의 초기 pbn 번호 세팅 */
			if (free_pbn > pbn) free_pbn = pbn;
		}
	}
	//
	// address mapping table 초기화 또는 복구
	// free block's pbn 초기화
		// address mapping table에서 lbn 수는 DATABLKS_PER_DEVICE 동일

	return;
}

//
// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 함
// (즉, 이 함수에서 메모리를 할당 받으면 안됨)
//
void ftl_read(int lsn, char* sectorbuf) {
	int lbn = lsn / PAGES_PER_BLOCK;
	int pbn = lbn_table[lbn];
	int offset = lsn % PAGES_PER_BLOCK;
	int ppn = pbn * PAGES_PER_BLOCK + offset;
	char* pagebuf = (char*)malloc(PAGE_SIZE + 1);
	dd_read(ppn, pagebuf);
	memcpy(sectorbuf, pagebuf, SECTOR_SIZE);
	free(pagebuf);
	return;
}

//
// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 함
// (즉, 이 함수에서 메모리를 할당 받으면 안됨)
//
void ftl_write(int lsn, char* sectorbuf) {
	int lbn = lsn / PAGES_PER_BLOCK;
	int pbn = lbn_table[lbn];
	int offset = lsn % PAGES_PER_BLOCK;
	int ppn;
	char* pagebuf = (char*)malloc(PAGE_SIZE + 1);

	/*저장하려는 lsn이 속한 lbn 블록의 최초 기록이라면 (free space 첫 페이지에 lbn 저장 과정 필요 O)*/
	if (pbn == -1) {

		/*lbn 테이블과 pbn 테이블 갱신*/
		pbn = free_pbn;
		lbn_table[lbn] = pbn;
		pbn_table[pbn] = lbn;

		/* 다음 free_block의 pbn을 찾는 과정. */
		free_pbn = DATABLKS_PER_DEVICE;
		for (int i = 0; i < DATABLKS_PER_DEVICE; i++) {
			if (pbn_table[i] == -1) {
				free_pbn = i;
				break;
			}
		}

		/*블록의 맨 처음 page의 spare에 lbn 저장*/
		dd_read(pbn * PAGES_PER_BLOCK, pagebuf);
		memcpy(pagebuf + SECTOR_SIZE, &lbn, sizeof(lbn));
		dd_write(pbn * PAGES_PER_BLOCK, pagebuf); //lbn이 저장된 page 데이터 flash file에 기록

		/*lsn에 대응하는 ppn에 데이터 저장*/
		ppn = pbn * PAGES_PER_BLOCK + offset;
		dd_read(ppn, pagebuf);
		memcpy(pagebuf, sectorbuf, SECTOR_SIZE);
		// spare에 lbn 저장
		memcpy(pagebuf + SECTOR_SIZE, &lbn, sizeof(lbn));	
		memcpy(pagebuf + SECTOR_SIZE + 4, &lsn, sizeof(lsn));
		
		dd_write(ppn, pagebuf);
	}
	/* 물리 데이터 블록의 최초 기록이 아닌 경우 (첫 페이지에 lbn 저장 과정 필요 X) */
	else {
		ppn = pbn * PAGES_PER_BLOCK + offset;
		dd_read(ppn, pagebuf);

		/*페이지 sector 영역의 4글자 읽어 lsn 추출
		(--> 페이지가 비어있는지 판단 기준) */
		
		int prev_lsn = 0;
		memcpy(&prev_lsn, pagebuf + SECTOR_SIZE + 4, sizeof(prev_lsn));
		/*해당 page가 비어있는 경우 --> 데이터 쓰기만 하면 OK*/
		if (prev_lsn == 0xffffffff) {
			memcpy(pagebuf, sectorbuf, SECTOR_SIZE);
			memcpy(pagebuf + SECTOR_SIZE, &lbn, sizeof(lbn));	
			memcpy(pagebuf + SECTOR_SIZE + 4, &lsn, sizeof(lsn));
			dd_write(ppn, pagebuf); //페이지 내용 쓰기.
		}

		/*해당 page가 비어있지 않은 경우 --> block 전체를 다른 곳으로 옮겨야함*/
		else {
			/* 데이터 블록에 여유공간이 있는 경우 */
			if (free_pbn != DATABLKS_PER_DEVICE) {
				int prev_pbn = pbn;
				pbn = free_pbn;
				lbn_table[lbn] = pbn;
				pbn_table[prev_pbn] = -1;
				pbn_table[pbn] = lbn;
				int prev_ppn; //이사전 오프셋의 ppn
				int cur_move_ppn; //이사후 오프셋의 ppn
				char* prev_page = (char*)malloc(PAGE_SIZE + 1); //이사시킬 오프셋의 페이지 내용


				/*덮어쓰려는 페이지 이전에 있는 페이지들의 데이터를 옮기는 과정*/
				for (int i = 0; i < PAGES_PER_BLOCK; i++) {
					prev_ppn = prev_pbn * PAGES_PER_BLOCK + i;
					cur_move_ppn = pbn * PAGES_PER_BLOCK + i;
					if (i == offset) { // 덮어쓰는 위치의 데이터인 경우 sector 내용 갱신 후 이동
						memcpy(pagebuf, sectorbuf, SECTOR_SIZE);
						dd_write(cur_move_ppn, pagebuf);
					}
					else { //아닌 경우 그대로 이동
						dd_read(prev_ppn, prev_page);
						dd_write(cur_move_ppn, prev_page);
					}
				}
				free(prev_page);

				dd_erase(prev_pbn);
				free_pbn = prev_pbn;
			}
			/* 데이터 블록에 여유 공간이 없는 경우 --> additional block 써야함 */
			else {
				int prev_ppn; //이사전 오프셋의 ppn
				int cur_move_ppn; //이사후 오프셋의 ppn
				char* prev_page = (char*)malloc(PAGE_SIZE + 1); //이사시킬 오프셋의 페이지 내용


				/*pbn의 내용을 extra free block에 옮기는 과정*/
				for (int i = 0; i < PAGES_PER_BLOCK; i++) {
					prev_ppn = pbn * PAGES_PER_BLOCK + i;
					cur_move_ppn = free_pbn * PAGES_PER_BLOCK + i;
					if (i == offset) { // 덮어쓰는 위치의 데이터인 경우 sector 내용 갱신 후 이동
						memcpy(pagebuf, sectorbuf, SECTOR_SIZE);
						dd_write(cur_move_ppn, pagebuf);
					}
					else { //아닌 경우 그대로 이동
						dd_read(prev_ppn, prev_page);
						dd_write(cur_move_ppn, prev_page);
					}
				}

				dd_erase(pbn);
				/*extra free block에 옮긴 내용을 다시 원래의 data block으로 옮기는 과정*/
				for (int i = 0; i < PAGES_PER_BLOCK; i++) {
					prev_ppn = free_pbn * PAGES_PER_BLOCK + i;
					cur_move_ppn = pbn * PAGES_PER_BLOCK + i;

					dd_read(prev_ppn, prev_page);
					dd_write(cur_move_ppn, prev_page);
				}
				dd_erase(free_pbn);
				free(prev_page);
			}
		}
	}

	free(pagebuf);
	return;
}

void ftl_print() {
	printf("lbn\tpbn\n");
	for (int i = 0; i < DATABLKS_PER_DEVICE; i++)
		printf("%d\t%d\n", i, lbn_table[i]);
	printf("free block's pbn=%d\n", free_pbn);
	return;
}
