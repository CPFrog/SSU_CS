#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mergeSort(double*, int, int); // 합병 정렬을 수행하는 함수
void merge(double*, int, int, int);

int num;

double temp[20000];

int main() {
	time_t start, end;
	int num; // 생성할 데이터의 갯수가 저장될 변수
	printf("랜덤으로 생성할 데이터 갯수를 입력하세요 : ");
	scanf("%d", &num);
	double arr[num]; // -1 ~ 1 사이의 실수가 저장될 배열
	start=clock();
	printf("초기 데이터 : ");
	for(int i=0;i<num;i++){
		srand(5+i); // 5부터 시작하는 시드값으로 고정시키기 위한 코드
		arr[i]=((double)(rand()%1000)/100)/10; // -1~1 사이의 실수가 나올 수 있도록 한 부분
		if(rand()%2==0) arr[i]*=-1;
		printf("%.3lf ", arr[i]);
	}
	printf("\n");
	
	mergeSort(arr, 0, num-1);
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<num;i++)
		printf("%.3lf ",*(arr+i));
	printf("\n");
	
	end=clock();
	printf("걸린 시간 : %ld ms\n", end-start);
	
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
}
