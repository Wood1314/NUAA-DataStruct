#include<stdlib.h>
#include<stdio.h>
#include<time.h>
int main()
{
	FILE *fp;
	//����0
	fp = fopen("data0.txt","w");
	if(fp == NULL){
		printf("open file erro\n");
	}
	for(int i=0; i<20000 ;i++){
		fprintf(fp,"%d ",i);
	}
    fclose(fp);
    fp = NULL;
    //����1
    fp = fopen("data1.txt", "w");
    if(fp == NULL){
		printf("open file erro\n");
	}
	for(int i=20000; i>=1; i--){
		fprintf(fp,"%d ",i);
	}
	fclose(fp);
	fp = NULL;
	
	//����2-9
	char fileName[20] = "datan.txt";
	for(int i=2; i<=9; i++){
		fileName[4] = '0'+i;
		fp = fopen(fileName, "w");
		if(fp == NULL){
			printf("open file erro\n");
		}
		srand((unsigned)time(NULL));		//��ʼ������� 
		for(int i=0; i<20000; i++){
			fprintf(fp,"%d ",rand()%30000); 
		}
		fclose(fp);
		fp = NULL;
	}
	return 0;
 } 
