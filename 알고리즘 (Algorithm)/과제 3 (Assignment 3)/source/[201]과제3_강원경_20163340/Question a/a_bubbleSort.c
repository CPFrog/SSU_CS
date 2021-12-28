#include <stdio.h>
#include <stdlib.h>

void bubbleSort(int*, int); // 버블 정렬을 수행하는 함수
void swap(int*, int*); // 배열의 원소 위치를 바꾸기 위한 함수

int main() {
	int num=8;
	int arr[]={12, 30, 21, 55, 25, 72, 45, 50};
	printf("초기 데이터 : ");
	for(int i=0;i<num;i++)
		printf("%d ", *(arr+i));
	printf("\n");
	
	bubbleSort(arr, num);
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<num;i++)
		printf("%d ",*(arr+i));
	printf("\n");
	
	return 0;
}

void bubbleSort(int *arr, int num){
	int step=1;
	for(int j=num-1;j>0;j--){ // 탐색 범위의 오른쪽 끝
		for(int i=0;i<j;i++){
			if(*(arr+i)>*(arr+i+1)) // i번째와 i+1번째 원소를 비교하여 필요에 따라 위치를 교환
				swap((arr+i),(arr+i+1));
		}
		// 처음 5회의 step을 출력하기 위한 부분
		if(step<6){
			printf("step %d : ", step);
			for(int k=0;k<num;k++)
				printf("%d ", *(arr+k));
			printf("\n");
		}
		step++;
	}
}

// 배열의 원소 위치를 맞바꾸기 위한 함수
void swap(int* a, int* b){
	int tmp=*a;
	*a=*b;
	*b=tmp;
}
