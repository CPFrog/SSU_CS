#include <stdio.h>
#include <stdlib.h>

void insertionSort(int*, int); // 삽입 정렬을 수행하는 함수

int main() {
	int num=8;
	int arr[]={12, 30, 21, 55, 25, 72, 45, 50};
	printf("초기 데이터 : ");
	for(int i=0;i<num;i++)
		printf("%d ", *(arr+i));
	printf("\n");
	
	insertionSort(arr, num);
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<num;i++)
		printf("%d ",*(arr+i));
	printf("\n");
	
	return 0;
}

void insertionSort(int *arr, int num){
	for(int j=1;j<num;j++){ // 탐색 범위의 오른쪽 끝
		for(int i=0;i<j;i++){
			if(*(arr+i)>*(arr+j)) {// i번째와 j번째 원소를 비교하여 j번째 원소의 삽입 위치를 결정
				int temp=*(arr+j);
				for(int k=j;k>i;k--)
					*(arr+k)=*(arr+k-1);
				*(arr+i)=temp;
			}
		}
		// 처음 5회의 step을 출력하기 위한 부분
		if(j<6){
			printf("step %d : ", j);
			for(int k=0;k<num;k++)
				printf("%d ", *(arr+k));
			printf("\n");
		}
	}
}
