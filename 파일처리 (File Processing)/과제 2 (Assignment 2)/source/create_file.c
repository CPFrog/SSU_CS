#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// 필요하면 header file 추가 가능.

int main(int argc, char **argv)
{
	char* record_num=argv[1];	
	FILE *fp = fopen(argv[2], "wb");
	int num=atoi(record_num);
	fwrite((const void*)&num, sizeof(int), 1, fp);

	for(int i=0;i<num;i++){
		char *c=(char*)malloc(sizeof(char)*250);
		memset(c,'a'+i%26,250);
		for(int j=0;j<250;j++){
			fwrite(c,sizeof(char),sizeof(c),fp);
		}
	}
	
	fclose(fp);
	return 0;
}
