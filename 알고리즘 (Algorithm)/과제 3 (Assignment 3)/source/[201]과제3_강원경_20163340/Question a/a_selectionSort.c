#include <stdio.h>
#include <stdlib.h>

void selectionSort(int*, int); // 선택 정렬을 수행하는 함수
void swap(int*, int*); // 배열의 원소 위치를 바꾸기 위한 함수

int main() {
	int num=8;
	int arr[]={12, 30, 21, 55, 25, 72, 45, 50};
	printf("초기 데이터 : ");
	for(int i=0;i<num;i++)
        printf("%d ", *(arr+i));
	printf("\n");
	
    selectionSort(arr, num);
    
	// 최종 수행 결과를 출력하는 부분
    printf("수행 결과 : ");
    for(int i=0;i<num;i++)
        printf("%d ",*(arr+i));
    printf("\n");
	
    return 0;
}

void selectionSort(int *a, int num){
    for(int i=0;i<num-1;i++){ // 탐색 범위의 왼쪽 끝
        int min=i; // 가장 작은 데이터의 인덱스 번호를 저장하는 변수
        for(int j=i+1;j<num;j++){
            if(*(a+min)>*(a+j)) // 현재 탐색 범위에서 가장 작은 값의 데이터인 경우 그 인덱스 번호를 기록
                min=j;
        }
        swap(a+i,a+min);
		// 처음 5회의 step을 출력하기 위한 부분
        if(i<5){
            printf("step %d : ", i+1);
            for(int k=0;k<num;k++)
                printf("%d ", *(a+k));
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
