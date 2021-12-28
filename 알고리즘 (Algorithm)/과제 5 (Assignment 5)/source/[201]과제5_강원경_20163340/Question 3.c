/* ----- 코드 작성 환경 -----
 * OS : macOS Big Sur (ver 11.6)
 * IDE : Xcode Version 13.0 (빌드 번호 : 13A233)
 * -----------------------
 */

#include <stdio.h>
#define INF 100000
#define NUM 7
#define MIN(a,b) ((a)<(b)?(a):(b))

int w[NUM][NUM]={{0,4,INF,INF,INF,10,INF}, {3,0,INF,18,INF,INF,INF}, {INF,6,0,INF,INF,INF,INF}, {INF,5,15,0,2,19,5}, {INF,INF,12,1,0,INF,INF}, {INF,INF,INF,INF,INF,0,10}, {INF,INF,INF,8,INF,INF,0}}; // 원본 그래프를 표현한 인접 행렬
int cache[NUM+1][1<<(NUM+1)]; // 해당 경로로 

int travel(int, int);
void traceRoute(int);

int main() {
    int ans=travel(0,1);
    printf("전체 순회시 최저비용 : %d\n",ans);
    
    printf("v1 -> ");
    traceRoute(ans);
    printf("v1\n");
    return 0;
}

// 최저 비용을 찾아 저장하는 함수
// cur: 현 위치의 점 번호, check: 방문한 정점의 체크가 된 비트마스크 변수
int travel(int cur, int check){
    if(check == (1<<NUM)-1) // 만약 모든 정점을 다 방문했다면
        return w[cur][0]; // v1으로 가는 비용 반환
    
    int *ret=&cache[cur][check];
    
    if (*ret!=0) return *ret; // ret 값이 있다는 것은 이미 방문한 적이 있는 정점이므로 저장된 값 반환.
    *ret=INF;
    for(int i=0;i<=NUM;i++){
        if(check&(1<<i)) continue;
        if(w[cur][i]==0) continue;
        *ret=MIN(*ret,travel(i, check|(1<<i))+w[cur][i]); // 가장 작은 값을 찾기 위한 과정.
    }

    return *ret;
}

// 최저 비용 순회 경로를 출력하는 함수.
// cost 에서 다음 경로를 뺀 값이 cache에 저장된 값과 동일하다면
// 그 경로가 가리키는 방향의 정점이 다음에 방문할 정점임에서 착안.
// cost: 현재 비용
void traceRoute(int cost){
    int piv=0, check=1;
    
    for(int i=0;i<=NUM;i++){
        for(int j=0;j<=NUM;j++){
            if(check&(1<<j)) continue;
            if(cost-w[piv][j]==cache[j][check+(1<<j)]){
                printf("v%d -> ", j);
                cost=cache[j][check+(1<<j)];
                piv=j;
                check+=(1<<j);
            }
        }
    }
}
