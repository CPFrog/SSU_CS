#include <stdio.h>
#include <stdlib.h>

void selectionSort(double*, int); // 선택 정렬을 수행하는 함수
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
	
	selectionSort(arr, 8); // 8개의 실수형 데이터의 삽입정렬 수행.
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<8;i++)
		printf("%.3lf ", arr[i]);
	printf("\n");
	
	return 0;
}

void selectionSort(double *a, int num){
	for(int i=0;i<num-1;i++){ // 탐색 범위의 왼쪽 끝
		int min=i; // 가장 작은 데이터의 인덱스 번호를 저장하는 변수
		for(int j=i+1;j<num;j++){
			if(0>*(a+j)-*(a+min)) // 현재 탐색 범위에서 가장 작은 값의 데이터인 경우 그 인덱스 번호를 기록
				min=j;
		}
		swap(a+i,a+min);
		// 처음 5회의 step을 출력하기 위한 부분
		if(i<5){
			printf("step %d : ", i+1);
			for(int k=0;k<num;k++)
				printf("%.3lf ", *(a+k));
			printf("\n");
		}
	}
}

// 배열의 원소 위치를 맞바꾸기 위한 함수
void swap(double* a, double* b){
	double tmp=*a;
	*a=*b;
	*b=tmp;
}
