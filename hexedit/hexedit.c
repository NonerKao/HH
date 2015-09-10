#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[]){
	FILE* fp = fopen(argv[1], "r+b");
	fseek(fp, atoi(argv[2]), SEEK_SET);
	fputc(atoi(argv[3]), fp);
	fclose(fp);
	return 0;
}
