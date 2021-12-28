#include <stdio.h>
#include <stdlib.h>

void insertionSort(double*, int); // 삽입 정렬을 수행하는 함수

int main() {
	int num=8;
	double arr[8]; // -1 ~ 1 사이의 실수 8개가 저장될 배열
	printf("랜덤한 숫자 8개 : ");
	for(int i=0;i<8;i++){
		srand(5+i); // 5부터 시작하는 시드값으로 고정시키기 위한 코드
		arr[i]=(5-(double)(rand()%1000)/100)/10; // -1~1 사이의 실수가 나올 수 있도록 한 부분
		printf("%.3lf ", arr[i]);
	}
	printf("\n");
	
	insertionSort(arr, 8);
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<num;i++)
		printf("%.3lf ",*(arr+i));
	printf("\n");
	
	return 0;
}

void insertionSort(double *arr, int num){
	for(int j=1;j<num;j++){ // 탐색 범위의 오른쪽 끝
		for(int i=0;i<j;i++){
			if(0>*(arr+j)-*(arr+i)) {// i번째와 j번째 원소를 비교하여 j번째 원소의 삽입 위치를 결정
				double temp=*(arr+j);
				for(int k=j;k>i;k--)
					*(arr+k)=*(arr+k-1);
				*(arr+i)=temp;
			}
		}
		// 처음 5회의 step을 출력하기 위한 부분
		if(j<6){
			printf("step %d : ", j);
			for(int k=0;k<num;k++)
				printf("%.3lf ", *(arr+k));
			printf("\n");
		}
	}
}
