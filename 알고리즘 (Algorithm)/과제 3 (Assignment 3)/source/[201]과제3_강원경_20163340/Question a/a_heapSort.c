#include <stdio.h>
#include <stdlib.h>

void heapSort(int*, int); // 재귀 호출 방식을 통해 퀵 정렬을 수행하는 함수
void swap(int*, int*); // 배열의 원소 위치를 바꾸기 위한 함수
void heapify(int*, int, int);

int num=8, step=0;

int main() {
	int arr[]={12, 30, 21, 55, 25, 72, 45, 50};
	printf("초기 데이터 : ");
	for(int i=0;i<num;i++)
		printf("%d ", *(arr+i));
	printf("\n");
	
	// leaf node가 아닌것부터 시작
	for (int i=num-1;i>=0;i--)
		heapify(arr,num,i);
	
	heapSort(arr,num);
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<num;i++)
		printf("%d ",*(arr+i));
	printf("\n");
	
	return 0;
}

// 힙 자료구조의 성질을 유지할 수 있도록 하는 함수
void heapify(int *arr, int num, int pos){
	int child;
	int left, right;
	
	do {
		left = (pos+1)*2-1;
		right = (pos+1)*2;
		
		if(left >= num && right >= num) break;
		else if(left>=num) child=right;
		else if(right>=num) child=left;
		else if(*(arr+left)>*(arr+right)) child=left;
		else child=right;
		
		if(*(arr+pos)<*(arr+child)) {
			swap(arr+pos,arr+child);
			pos=child;
		}
		else break;
	} while(left<num);
}

void heapSort(int *arr, int num){
	for (int i=num-1; i>=0; i--) {
		swap(arr,arr+i);
		heapify(arr,i,0);
		
		if(step++ < 5) {
			printf("step %d : ", step);
			for (int j = 0; j < num; j++) {
				printf("%d ", arr[j]);
			}
			printf("\n");
		}
	}
}

// 배열의 원소 위치를 맞바꾸기 위한 함수
void swap(int* a, int* b){
	int tmp=*a;
	*a=*b;
	*b=tmp;
}
