// 코드 작성 환경 정보
// OS : macOS (Big Sur) 11.5.2
// IDE 환경 : Xcode 12.5.1

#include <stdio.h>
#include <stdlib.h>

int binarySearch(int, int, int*, int);

int main(){
	int num; //배열의 원소 갯수
	scanf("%d", &num);
	
	int *arr=(int*)malloc(sizeof(int)*num); //정렬된 데이터들을 읽어들일 배열을 동적할당.
	
	for(int i=0;i<num;i++)
		scanf("%d",arr+i);
	
	int search; //찾으려는 데이터의 값
	scanf("%d",&search);
	
	int left=0, right=num-1; // 왼쪽 끝의 인덱스 번호, 오른쪽 끝의 인덱스 번호.
	
	int ans=binarySearch(left, right, arr, search); // 재귀적 이진탐색을 수행한 결과를 저장하는 변수.
	
	if(ans!=-1) // 검색 결과 분석 = -1일 경우 검색 실패, 아닐 경우 데이터가 저장된 인덱스 번호.
		printf("%d\n",ans+1); //문제 명세상 인덱스의 시작을 1로 두는 것 같아 (인덱스번호)+1 출력.
	else
		printf("실패\n");
	return 0;
}

/* ===================================================
 재귀호출 방법을 이용해 이진 탐색을 수행하는 함수.
 
 left : 찾는 범위의 왼쪽 끝 인덱스 번호
 right : 찾는 범위의 오른쪽 끝 인덱스 번호
 arr : 검색대상이 되는 데이터들이 저장된 배열 주소
 search : 사용자가 찾으려는 값
 리턴값 : 찾으려는 값이 있는 경우 그 데이터의 인덱스 번호, 없는 경우 -1
 =================================================== */
int binarySearch(int left, int right, int *arr, int search){
	int mid=(left+right)/2;
	
	if(*(arr+mid)==search) //같을 경우
		return mid;
	else if(left<right){
		if(*(arr+mid)<search) //다를 경우
			return binarySearch(mid+1, right, arr, search);
		else
			return binarySearch(left, mid, arr, search);
	}
	else return -1;
}
