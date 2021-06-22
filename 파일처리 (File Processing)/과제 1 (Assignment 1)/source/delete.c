#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int count;
	char buffer[10];

	FILE *fp1 = fopen(argv[1],"r+");
	long offset = atol(argv[2]);
	long delete = atol(argv[3]);
	FILE *fp2 = fopen("temp","w");
	
	long curlen=0;
	while(feof(fp1) == 0){
		count=fread(buffer,sizeof(char),9,fp1);
		if(curlen+(long)count>offset){
			int curcnt=(int)(offset-curlen);
			fwrite(buffer,sizeof(char),curcnt,fp2);
			curlen=offset;
			memset(buffer,0,10);
			break;
		}
		else { 
			fwrite(buffer,sizeof(char),count,fp2);
			curlen+=count;
			memset(buffer,0,10);
		}
	}
	fp2=fopen("temp","a");
	fseek(fp1,delete,0);
	while(feof(fp1)==0){
		count=fread(buffer,sizeof(char),9,fp1);
		fwrite(buffer,sizeof(char),count,fp2);
		memset(buffer,0,10);
	}
	
	fclose(fp1);
	fclose(fp2);
	remove(argv[1]);
	rename("temp",argv[1]);
	
	return 0;
}
