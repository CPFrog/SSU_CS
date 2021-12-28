#include <stdio.h>
#include <stdlib.h>

void quickSort(double*, int, int); // 재귀 호출 방식을 통해 퀵 정렬을 수행하는 함수
void swap(double*, double*); // 배열의 원소 위치를 바꾸기 위한 함수
int partition(double*, int, int); // 배열의 pivot을 중심으로 해당 단계의 퀵정렬을 수행하는 함수. (Divide)

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
	
	quickSort(arr, 0, num-1);
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<num;i++)
		printf("%.3lf ",*(arr+i));
	printf("\n");
	
	return 0;
}

void quickSort(double *arr, int left, int right){
	if(left<right){
		int pivot=partition(arr,left,right); // 현재 step에서의 퀵 정렬을 수행
		
		quickSort(arr, left, pivot-1); // 피벗보다 작은 수들의 부분만을 퀵 정렬 수행
		quickSort(arr, pivot+1, right); // 피벗보다 큰 수들의 부분만을 퀵 정렬 수행
	}
}

// 현재 step에서의 퀵 정렬을 수행하고, 피벗이 위치한 인덱스를 반환하는 함수
int partition(double *arr, int left, int right){
	double pivot=arr[right];
	int l=left-1, r=right;
	
	// 왼쪽 끝과 오른쪽 끝이 교차할때까지 반복
	do{
		// 왼쪽 끝이 피벗보다 작을 경우 계속 증가
		do{
			l++;
		}while((l<=right)&&(0<pivot-arr[l]));
		
		// 오른쪽 끝이 피벗보다 클 경우 계속 감소
		do{
			r--;
		}while((r>=left)&&(0>pivot-arr[r]));
		
		// 양 끝이 교차하지 않았을 경우, 왼쪽 오른쪽 부분의 조건에 어긋난 두 데이터의 위치를 교환
		if(l<r)
			swap(arr+l,arr+r);
	} while(l<r);
	
	swap(arr+right, arr+l);
	
	step++;
	
	// 처음 5개의 step을 출력하기 위한 부분
	if(step<6){
		printf("step %d : ", step);
		for(int i=0;i<num;i++)
			printf("%.3lf ", *(arr+i));
		printf("\n");
	}
	
	
	return l; //함수 수행 후 피벗이 위치한 인덱스를 반환.
}

// 배열의 원소 위치를 맞바꾸기 위한 함수
void swap(double* a, double* b){
	double tmp=*a;
	*a=*b;
	*b=tmp;
}
