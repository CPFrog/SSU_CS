#include <stdio.h>
#include <time.h>
// 필요시 header file 추가 가능.

int main(int argc, char **argv)
{
	FILE *fp=fopen(argv[1],"rb");
	
	int num;
	fread(&num, sizeof(int), 1, fp);
	clock_t start=clock();
	char record[252];
	for(int i=0;i<num;i++){
		fread(record, sizeof(char),250,fp);
	}
	clock_t end=clock();
	fclose(fp);
	printf("#records: %d elapsed_time: %ld us\n", num, end-start);

	return 0;
}
