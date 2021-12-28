#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insertionSort(double*, int); // 삽입 정렬을 수행하는 함수

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
	
	insertionSort(arr, num);
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<num;i++)
		printf("%.3lf ",*(arr+i));
	printf("\n");
	
	end=clock();
	printf("걸린 시간 : %ld ms\n", end-start);
	
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
	}
}
