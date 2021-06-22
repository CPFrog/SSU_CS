#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	int count;
	char buffer[10];

	memset(buffer,0,10);

	FILE *fp1 = fopen(argv[1],"r");
	FILE *fp2 = fopen(argv[2], "w");

	while(feof(fp1) ==0){
		count=fread(buffer,sizeof(char),9,fp1);
		fwrite(buffer,sizeof(char),count,fp2);
		memset(buffer,0,10);
	}

	fclose(fp1);
	fclose(fp2);

	return 0;
}

