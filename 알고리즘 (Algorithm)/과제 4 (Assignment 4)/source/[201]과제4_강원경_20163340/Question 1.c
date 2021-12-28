#include <stdio.h>
#include <stdlib.h>
#define INF 1000000000 // 간선이 없는 두 정점간의 거리는 INF로 처리.

void floyd(int, int*, int*);

const int w[7][7] = {{INF,4,INF,INF,INF,10,INF}, {3,INF,INF,18,INF,INF,INF}, {INF,6,INF,INF,INF,INF,INF}, {INF,5,15,INF,2,19,5}, {INF,INF,12,1,INF,INF,INF}, {INF,INF,INF,INF,INF,INF,10}, {INF,INF,INF,8,INF,INF,INF}}; // 원본 그래프는 const 전역변수로 선언

int main() {
    int* d=(int*)calloc(sizeof(int), 49); // 두 정점간의 거리를 저장하게 될 배열을 1차원 배열 형태로 동적할당.
    int* p=(int*)calloc(sizeof(int), 49); // 정점간 최단거리 경로상의 경유지점을 저장할 배열을 1차원 배열 형태로 동적할당.
    
    floyd(7,d,p);
}

// 플로이드 알고리즘 함수
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
        
		// 각 step (경유 정점 바뀌는 시점)마다 결과 출력하는 코드
		printf("-- step %d (정점 v%d을 거칠 때) --\n행렬 D :\n", k+1, k+1);
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++){
                if(*(d+i*7+j)==INF)
                    printf("INF ");
                else printf("%3d ", *(d+i*7+j));
            }
            printf("\n");
        }
        
        printf("\n행렬 P :\n");
        for(int i=0;i<7;i++){
            for(int j=0;j<7;j++)
                printf("%d ", *(p+i*7+j));
            printf("\n");
        }
        printf("\n\n");
    }
}
