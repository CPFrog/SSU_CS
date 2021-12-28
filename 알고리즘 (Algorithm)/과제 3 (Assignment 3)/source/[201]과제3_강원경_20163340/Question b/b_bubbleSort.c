#include <stdio.h>
#include <stdlib.h>

void bubbleSort(double*, int); // 버블 정렬을 수행하는 함수
void swap(double*, double*); // 배열의 원소 위치를 바꾸기 위한 함수

int main() {
	double arr[8]; // -1 ~ 1 사이의 실수 8개가 저장될 배열
	printf("랜덤한 숫자 8개 : ");
	for(int i=0;i<8;i++){
		srand(5+i); // 5부터 시작하는 시드값으로 고정시키기 위한 코드
		arr[i]=(5-(double)(rand()%1000)/100)/10; // -1~1 사이의 실수가 나올 수 있도록 한 부분
		printf("%.3lf ", arr[i]);
	}
	printf("\n");
	
	bubbleSort(arr, 8);
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<8;i++)
		printf("%.3lf ",*(arr+i));
	printf("\n");
	
	return 0;
}

void bubbleSort(double *arr, int num){
	int step=1;
	for(int j=num-1;j>0;j--){ // 탐색 범위의 오른쪽 끝
		for(int i=0;i<j;i++){
			if(0>*(arr+i+1)-*(arr+i)) // i번째와 i+1번째 원소를 비교하여 필요에 따라 위치를 교환
				swap((arr+i),(arr+i+1));
		}
		// 처음 5회의 step을 출력하기 위한 부분
		if(step<6){
			printf("step %d : ", step);
			for(int k=0;k<num;k++)
				printf("%.3lf ", *(arr+k));
			printf("\n");
		}
		step++;
	}
}

// 배열의 원소 위치를 맞바꾸기 위한 함수
void swap(double* a, double* b){
	double tmp=*a;
	*a=*b;
	*b=tmp;
}
