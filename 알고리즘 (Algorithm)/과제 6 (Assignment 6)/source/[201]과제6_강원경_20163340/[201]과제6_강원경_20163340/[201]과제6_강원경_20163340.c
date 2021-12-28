#include <stdio.h>
#include <stdlib.h>

#define INF 10000 // MST에서 해당 단계에 갈 수 없는 정점에 대한 비용을 무한대로 설정하기 위해 충분히 큰 값으로 설정
#define NUM 10 // 배열의 최대,최소크기

const int W[10][10] = {{0,32,0,17,0,0,0,0,0,0},{32,0,0,0,45,0,0,0,0,0},{0,0,0,18,0,0,5,0,0,0},{17,0,18,0,10,0,0,3,0,0},{0,45,0,10,0,28,0,0,25,0},{0,0,0,0,28,0,0,0,0,6},{0,0,5,0,0,0,0,59,0,0},{0,0,0,3,0,0,59,0,4,0},{0,0,0,0,25,0,0,4,0,12},{0,0,0,0,0,6,0,0,12,0}}; // 원본 그래프를 인접 배열 형태로 표현.

// Prim 알고리즘에서 사용되는 변수 & 함수
int prim(int); // 프림 알고리즘을 수행하는 함수
int spanning[NUM][NUM]; // [prim] 최소신장트리의 집합으로 들어갈 정점사이의 비용이 저장되는 배열

// Kruskal 알고리즘에 사용되는 변수 & 함수
int p[NUM*NUM]; // [kruskal] 해당 노드가 속한 그룹의 촤상위 부모노드 번호 저장 배열
int kruskal(void); // 크루스칼 알고리즘을 수행하는 함수
int cmp(const void *, const void *); // 간선 구조체의 정렬 시 오름차순 정렬에 사용되는 비교함수
int findParent(int); // 대표 정점을 찾기 위한 함수
void unite(int, int); // 하나의 그래프로 합치기 위한 함수
struct _edge{
	int start, end;
	int value;
} typedef edge;

int main(){
    int startPoint, totalCost; // 시작 정점 번호, MST의 총 비용
    // 문제 1
    startPoint=1; // 탐색 시작 정점 번호: v1
    printf("----- 문제 1 수행 결과 -----\n");
    totalCost=prim(startPoint-1); // 정점 번호는 1부터 시작하지만, 인덱스는 0부터 시작하므로 (시작 정점 번호-1) 수행.
    printf("[생성된 MST의 간선]\n");

    for(int i=0;i<NUM;i++) {
        for(int j=i;j<NUM;j++){ // 방향성 없는 간선의 중복 출력 방지 위해 j=i부터 시작.
            if(spanning[i][j]!=0)
                printf("v%02d - v%02d : %d\n", i+1, j+1, spanning[i][j]);
        }
    }

    printf("생성된 MST의 총 비용 = %d\n\n",totalCost);
    
    // 문제2
    startPoint=8; // 탐색 시작 정점 번호: v8
    printf("----- 문제 2 수행 결과 -----\n");
    totalCost=prim(startPoint-1); // 정점 번호는 1부터 시작하지만, 인덱스는 0부터 시작하므로 (시작 정점 번호-1) 수행.
    printf("[생성된 MST의 간선]\n");

    for(int i=0;i<NUM;i++) {
        for(int j=i;j<NUM;j++){ // 방향성 없는 간선의 중복 출력 방지 위해 j=i부터 시작.
            if(spanning[i][j]!=0)
                printf("v%02d - v%02d : %d\n", i+1, j+1, spanning[i][j]);
        }
    }

    printf("생성된 MST의 총 비용 = %d\n\n",totalCost);
    
    // 문제3
    printf("----- 문제 3 수행 결과 -----\n");
    totalCost=kruskal();

    printf("생성된 MST의 총 비용 = %d\n\n",totalCost);
    
    return 0;
}

// 프림 알고리즘을 수행하는 함수
// start: 시작 정점 번호
// 반환값: 해당 정점 번호에서 시작해 만든 MST의 전체 비용
int prim(int start) {
    int cost[NUM][NUM]; // i->j로 가는 비용이 저장되는 배열. 연결 불가능할 시 INF
    int u,v,minDist,dist[NUM],from[NUM]; //정점의 시작점, 정점의 끝점, 최소거리, 거리의 최대
    int visitCheck[NUM],edgeCount,minCost; //각 정점을 방문했는지 여부, MST의 간선 개수, MST의 전체 비용

    //정점사이의 비용이 저장되는 배열과 집합에서의 최소비용을 저장하는 배열 초기화
    for(int i=0;i<NUM;i++){
        for(int j=0;j<NUM;j++){
            if(W[i][j]==0)
                cost[i][j]=INF;
            else
                cost[i][j]=W[i][j];
                spanning[i][j]=0;
        }
    }

    //출발 정점과 방문했던 정점과 거리를 초기화
    dist[start]=0;
    visitCheck[start]=1;

    for(int i=1;i<NUM;i++) {
        int cur=(start+i)%NUM;
        dist[cur]=cost[start][cur];
        from[cur]=start;
        visitCheck[cur]=0;
    }

    minCost=0; // 전체 비용은 0부터 시작.
    edgeCount=NUM-1; //MST의 간선 개수는 정점개수-1 이므로 이에 해당하는 값으로 초기화.

    while(edgeCount>0) {
        //트리로부터 최소비용거리를 가진 정점들을 찾음
        minDist=INF;
        for(int i=1;i<NUM;i++){
            int cur=(start+i)%NUM;
            if(visitCheck[cur]==0&&dist[cur]<minDist) {
                v=cur;
                minDist=dist[cur];
            }
        }

        u=from[v];

        // spanning tree에 최소 비용의 간선을 삽입
        spanning[u][v]=dist[v];
        spanning[v][u]=dist[v];
        edgeCount--;
        visitCheck[v]=1;

        // 최소 비용의 간선들에서 거친 정점들을 다시 배열에 정리
        for(int i=1;i<NUM;i++){
            int cur=(start+i)%NUM;
            if(visitCheck[cur]==0&&cost[cur][v]<dist[cur]){
                dist[cur]=cost[cur][v];
                from[cur]=v;
            }
        }

        minCost+=cost[u][v];
    }

    return(minCost);
}

// 크루스칼 알고리즘 수행 함수.
int kruskal(void){
    edge eArr[NUM*(NUM-1)/2]; // 원래 그래프에서 가중치가 있는 간선들이 모두 저장되는 배열.
	// 최대 간선 개수가 nC2라는 점에서 배열의 크기 착안.
    int cnt=0;
	
	// 배열 초기화 부분
	p[0]=0;
    for(int i=0;i<NUM-1;i++){
		p[i+1]=i+1;
        for(int j=i+1;j<NUM;j++){ // 원래 그래프의 간선들의 정보를 배열에 저장
            if(W[i][j]!=0){
                edge tmp;
                tmp.start=i;
                tmp.end=j;
                tmp.value=W[i][j];
                eArr[cnt++]=tmp;
            }
        }
    }
	
    qsort(eArr,cnt,sizeof(edge),cmp); // stdlib.h에 있는 quicksort를 사용하여 정렬.
    
	// 비용이 작은 간선들을 하나씩 선택해가며 속한 그룹이 다를 경우(최상위 부모 노드가 다를 경우)에만 간선을 선택하여 연결.
	int minCost=0; // MST의 전체 비용이 저장되는 변수
	printf("[생성된 MST의 간선]\n");
	for(int i=0;i<cnt;i++){
		edge e=eArr[i];
		int x=findParent(eArr[i].start);
		int y=findParent(eArr[i].end);
		if(x!=y){ //x와 y가 속한 그룹이 다르다면 병합한 후 병합의 매개체가 되는 간선 출력
			unite(eArr[i].start, eArr[i].end);
			printf("v%02d - v%02d : %d\n", e.start+1, e.end+1, e.value);
			minCost+=eArr[i].value;
		}
	}
    return minCost;
}

// 구조체의 대소비교를 위한 비교 함수
// a, b: 비교할 두 구조체를 가리키는 포인터.
int cmp(const void *a, const void *b){
    edge *pa=(edge*)a;
    edge *pb=(edge*)b;
    return pa->value > pb->value? 1:-1;
}

// 자신이 속한 그룹의 최상위 부모 노드가 무엇인지 검색.
// x: 현재 노드의 번호.
// return 값: 최상위 부모 정점 번호.
int findParent(int x){
	if(x==p[x])
		return x;
	else
		return p[x]=findParent(p[x]);
}

// 두 정점의 최상위 부모 노드를 일치시킴으로써 하나의 그룹으로 병합.
// x, y: 같은 그룹에 속해있는지 판단의 대상이 되는 두 개의 정점 번호.
void unite(int x, int y){
	x=findParent(x);
	y=findParent(y);
	p[x]=y;
}
