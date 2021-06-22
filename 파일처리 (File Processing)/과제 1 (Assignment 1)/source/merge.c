#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	int count;
	char buffer[10];
	memset(buffer,0,10);
	
	FILE *fp1 = fopen(argv[1], "w");
	FILE *fp2 = fopen(argv[2], "r");

	while(feof(fp2) ==0){
		count=fread(buffer,sizeof(char),9,fp2);
		fwrite(buffer,sizeof(char),count,fp1);
		memset(buffer,0,10);
	}
	fclose(fp2);

	FILE *fp3 = fopen(argv[3], "r");
	while(feof(fp3) ==0){
		count=fread(buffer,sizeof(char),9,fp3);
		fwrite(buffer,sizeof(char),count,fp1);
		memset(buffer,0,10);
	}

	fclose(fp3);	
	fclose(fp1);

	return 0;
}
