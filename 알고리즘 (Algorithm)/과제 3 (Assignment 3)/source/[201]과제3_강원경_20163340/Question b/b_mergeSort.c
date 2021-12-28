#include <stdio.h>
#include <stdlib.h>

void mergeSort(double*, int, int); // 합병 정렬을 수행하는 함수
void merge(double*, int, int, int);
void printStep(double*, int);

int step=1, num=8;

double temp[8];

int main() {
	double arr[8]; // -1 ~ 1 사이의 실수 8개가 저장될 배열
	printf("랜덤한 숫자 8개 : ");
	for(int i=0;i<8;i++){
		srand(5+i); // 5부터 시작하는 시드값으로 고정시키기 위한 코드
		arr[i]=(5-(double)(rand()%1000)/100)/10; // -1~1 사이의 실수가 나올 수 있도록 한 부분
		printf("%.3lf ", arr[i]);
	}
	printf("\n");
	
	mergeSort(arr, 0, num-1);
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<num;i++)
		printf("%.3lf ",*(arr+i));
	printf("\n");
	
	return 0;
}

void mergeSort(double *arr, int left, int right){
	if(left<right){
		int mid=(left+right)/2;
		mergeSort(arr, left, mid);
		mergeSort(arr, mid+1, right);
		merge(arr, left, mid, right);
	}
}

void merge(double *arr, int left, int mid, int right){
	int l=left, r=mid+1, cur=left;
	
	while(l<=mid && r<=right){
		if(0<=arr[r]-arr[l])
			temp[cur++]=arr[l++];
		else
			temp[cur++]=arr[r++];
	}
		
	if(l>mid){
		for(int i=r;i<=right;i++)
			temp[cur++]=arr[i];
	}
	
	else{
		for(int i=l;i<=mid;i++)
			temp[cur++]=arr[i];
	}
	
	for(int i=left;i<=right;i++){
		arr[i]=temp[i];
	}
	if(step<6)
		printStep(arr, num);
	step++;
}

void printStep(double *arr, int num){
	printf("step %d : ", step);
	for(int i=0;i<num;i++)
		printf("%.3lf ", arr[i]);
	printf("\n");
}
