#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
//필요하면 header file 추가 가능.

#define SUFFLE_NUM	100000	// <- 임의의 값으로 변경 가능.

void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);
// 필요한 함수가 있으면 더 추가할 수 있음.


int main(int argc, char **argv)
{
	FILE *fp=fopen(argv[1], "rb");

	int *read_order_list;
	int num_of_records; 
	fread(&num_of_records, sizeof(int), 1, fp);
	read_order_list=(int*)malloc(sizeof(int)*num_of_records);	

	clock_t start=clock();

	// 이 함수를 실행하면 'read_order_list' 배열에는 읽어야 할 레코드 번호들이 나열되어 저장됨.
	GenRecordSequence(read_order_list, num_of_records);
	
	char record[252];
	for(int i=0;i<num_of_records;i++){
		int pos=read_order_list[i];
		fseek(fp,pos*250,SEEK_SET);
		fread(record,sizeof(char),250,fp);
	}
	
	clock_t end=clock();
	printf("#records: %d elapsed_time: %ld us\n", num_of_records, end-start);
	fclose(fp);
	return 0;
}

void GenRecordSequence(int *list, int n)
{
	int i,j,k;
	srand((unsigned int)time(0));
	for(i=0; i<n; i++)
	{
		list[i] = i;
	}
	for(i=0; i<SUFFLE_NUM; i++)
	{
		j = rand() % n;
		k = rand() % n;
		swap(&list[j], &list[k]);
	}
}

void swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
