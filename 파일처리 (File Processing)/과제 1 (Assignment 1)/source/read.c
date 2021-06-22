#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int count;
	long cur_byte=0;
	char buffer[10];
	memset(buffer,0,10);
	
	FILE *fp = fopen(argv[1],"r");
	long offset=atoi(argv[2]);
	long read_byte=atoi(argv[3]);
	
	fseek(fp, offset, SEEK_SET);
	
	while(feof(fp)==0){
		count=fread(buffer,sizeof(char),9,fp);
		if(cur_byte+count<=read_byte) 
			printf("%s",buffer);
		else{
			int i;
			for(i=0;i<read_byte-cur_byte;i++){
				printf("%c",buffer[i]);
			}
			break;
		}
		cur_byte+=count;
		memset(buffer,0,10);
	}

	fclose(fp);
	printf("\n");

	return 0;
}
