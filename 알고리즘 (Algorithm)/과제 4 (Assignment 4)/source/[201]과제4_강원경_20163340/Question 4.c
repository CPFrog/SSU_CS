#include <stdio.h>
#include <stdlib.h>
#define INF 1000000000 // 간선이 없는 두 정점간의 거리는 INF로 처리.

const int w[7][7] = {{INF,INF,INF,INF,INF,INF,10}, {INF,INF,1,12,INF,INF,INF}, {19,2,INF,15,5,INF,5}, {INF,INF,INF,INF,6,INF,INF}, {INF,INF,18,INF,INF,3,INF}, {10,INF,INF,INF,4,INF,INF}, {INF,INF,8,INF,INF,INF,INF}}; // 원본 그래프는 const 전역변수로 선언

void floyd(int, int*, int*);
void path(int*, int, int, int*, int*);

int main(){
	int* d=(int*)calloc(sizeof(int), 49); // 두 정점간의 거리를 저장하게 될 배열을 1차원 배열 형태로 동적할당.
	int* p=(int*)calloc(sizeof(int), 49); // 정점간 최단경로상의 경유지점을 저장할 배열을 1차원 배열 형태로 동적할당.
	
	floyd(7,d,p);
	
	int start=7, end=4; // 탐색하려는 정점의 시작점과 끝점
	int* res=(int*)calloc(sizeof(int), 7); // 최단경로상 거치게 되는 정점들의 번호를 저장하는 배열.
	int num=0; // 최단경로상 거치게 되는 정점들의 갯수
	
	
	path(p,start-1,end-1,res,&num); // 배열의 index가 0부터 시작하기 때문에 정점 번호를 -1함.
	
	printf("\n최단경로 : a%d -> ",start);
	for(int i=0;i<num;i++)
		printf("a%d -> ", *(res+i));
	printf("a%d\n",end);
	return 0;
}

// 행렬 P를 생성하기 위한 플로이드 알고리즘 함수
// n: 그래프의 정점 갯수, d: 정점간 최단거리 경로의 길이가 저장된 배열, p: 정점간 최단거리 경로상의 경유지점이 저장된 배열.
void floyd(int n, int* d, int* p){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			*(p+i*7+j)=0;
			*(d+i*7+j)=w[i][j];
		}
	}
	
	for(int k=0;k<n;k++){
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				if(*(d+i*7+k)+*(d+k*7+j)<*(d+i*7+j)){ //정점 k를 거치는 경로가 훨씬 짧다면
					*(p+i*7+j)=k; // 경유 정점 k를 배열 p에 저장
					*(d+i*7+j)=*(d+i*7+k)+*(d+k*7+j); // 최단거리에 대한 길이 갱신
				}
			}
		}
	}
}

// 최단 경로를 출력하는 함수.
// p: 최단거리 경로상 경유 정점이 저장된 배열, q: 간선 시작점, r: 간선 종점
// res: 경유하게 되는 모든 정점이 저장된 배열, i: 경유하게 되는 정점들의 갯수가 저장된 변수를 가리키는 포인터 변수.
void path(int* p, int q, int r, int* res, int* i){
	printf("path(%d,%d) = %d\n",q,r,*(p+q*7+r));
	if(*(p+q*7+r)!=0){
		path(p, q,*(p+q*7+r), res, i);
		printf("%d번째 경유 정점 : a%d\n", *(i)+1, *(p+q*7+r)+1);
		*(res+*(i))=*(p+q*7+r)+1;
		*(i)+=1;
		path(p, *(p+q*7+r), r, res, i);
	}
}
