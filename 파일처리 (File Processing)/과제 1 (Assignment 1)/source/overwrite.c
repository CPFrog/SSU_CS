#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int count;

	FILE *fp1 = fopen(argv[1],"r+");
	long offset = atoi(argv[2]);
	char *data = argv[3];
	
	fseek(fp1,offset,SEEK_SET);
	fwrite(data,sizeof(char),strlen(data),fp1);
	
	fclose(fp1);


	return 0;
}
