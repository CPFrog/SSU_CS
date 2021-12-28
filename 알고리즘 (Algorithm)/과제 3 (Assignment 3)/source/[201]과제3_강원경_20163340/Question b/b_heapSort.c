#include <stdio.h>
#include <stdlib.h>

void heapSort(double*, int); // 재귀 호출 방식을 통해 퀵 정렬을 수행하는 함수
void swap(double*, double*); // 배열의 원소 위치를 바꾸기 위한 함수
void heapify(double*, int, int);

int num=8, step=0;

int main() {
	double arr[8]; // -1 ~ 1 사이의 실수 8개가 저장될 배열
	printf("랜덤한 숫자 8개 : ");
	for(int i=0;i<8;i++){
		srand(5+i); // 5부터 시작하는 시드값으로 고정시키기 위한 코드
		arr[i]=(5-(double)(rand()%1000)/100)/10; // -1~1 사이의 실수가 나올 수 있도록 한 부분
		printf("%.3lf ", arr[i]);
	}
	printf("\n");
	
	// leaf node가 아닌것부터 시작
	for (int i=num-1;i>=0;i--)
		heapify(arr,num,i);
	
	heapSort(arr,num);
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<num;i++)
		printf("%.3lf ",*(arr+i));
	printf("\n");
	
	return 0;
}

// 힙 자료구조의 성질을 유지할 수 있도록 하는 함수
void heapify(double *arr, int num, int pos){
	int child;
	int left, right;
	
	do {
		left = (pos+1)*2-1;
		right = (pos+1)*2;
		
		if(left >= num && right >= num) break;
		else if(left>=num) child=right;
		else if(right>=num) child=left;
		else if(0>*(arr+right)-*(arr+left)) child=left;
		else child=right;
		
		if(0<*(arr+child)-*(arr+pos)) {
			swap(arr+pos,arr+child);
			pos=child;
		}
		else break;
	} while(left<num);
}

void heapSort(double *arr, int num){
	for (int i=num-1; i>=0; i--) {
		swap(arr,arr+i);
		heapify(arr,i,0);
		
		if(step++ < 5) {
			printf("step %d : ", step);
			for (int j = 0; j < num; j++) {
				printf("%.3lf ", arr[j]);
			}
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
