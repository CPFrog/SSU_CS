#include <stdio.h>
#include <stdlib.h>

void mergeSort(int*, int, int); // 합병 정렬을 수행하는 함수
void merge(int*, int, int, int);
void printStep(int*, int);

int lv=1, num=8;

int temp[20000];

int main() {
	int arr[]={12, 30, 21, 55, 25, 72, 45, 50};
	printf("초기 데이터 : ");
	for(int i=0;i<num;i++)
		printf("%d ", *(arr+i));
	printf("\n");
	
	mergeSort(arr, 0, num-1);
	
	// 최종 수행 결과를 출력하는 부분
	printf("수행 결과 : ");
	for(int i=0;i<num;i++)
		printf("%d ",*(arr+i));
	printf("\n");
	
	return 0;
}

void mergeSort(int *arr, int left, int right){
	if(left<right){
		int mid=(left+right)/2;
		mergeSort(arr, left, mid);
		mergeSort(arr, mid+1, right);
		merge(arr, left, mid, right);
	}
}

void merge(int arr[], int left, int mid, int right){
	int l=left, r=mid+1, cur=left;
	
	while(l<=mid && r<=right){
		if(arr[l]<=arr[r])
			temp[cur++]=arr[l++];
		else
			temp[cur++]=arr[r++];
	}
		
	if(l>mid){
		for(int i=r;i<=right;i++)
			temp[cur++]=arr[i];
	}
	
	else{
		for(int i=l;i<=mid;i++)
			temp[cur++]=arr[i];
	}
	
	for(int i=left;i<=right;i++){
		arr[i]=temp[i];
	}
	if(lv<6) // 처음 5개의 step 과정을 출력하기 위한 코드
		printStep(arr, num);
	lv++;
}

void printStep(int *arr, int num){
	printf("step %d : ", lv);
	for(int i=0;i<num;i++)
		printf("%d ", arr[i]);
	printf("\n");
}
