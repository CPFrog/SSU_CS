#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int count;
	char buffer[10];

	FILE *fp1 = fopen(argv[1],"r+");
	long offset = atol(argv[2]);
	char *data = argv[3];
	FILE *fp2 = fopen("temp","w+");
	
	while(feof(fp1) ==0){
		count=fread(buffer,sizeof(char),9,fp1);
		fwrite(buffer,sizeof(char),count,fp2);
		memset(buffer,0,10);
	}
	
	fseek(fp1,offset,SEEK_SET);
	fwrite(argv[3],sizeof(char),strlen(argv[3]),fp1);
	fseek(fp2,offset,0);
	while(feof(fp2)==0){
		count=fread(buffer,sizeof(char),9,fp2);
		fwrite(buffer,sizeof(char),count,fp1);
		memset(buffer,0,10);
	}
	
	fclose(fp2);
	remove("temp");
	fclose(fp1);
	
	return 0;
}
