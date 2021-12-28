/*
 * [201]과제7_강원경_20163340
 * ----- 코드 작성 환경 -----
 * OS : macOS Monterey (ver 12.0.1)
 * IDE : Xcode Version 13.1 (빌드 번호 : 13A1030d)
 * -----------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 문제 11에 사용되는 함수
int queen_promising(int, int*);
int estimate_n_queens(int);

// 문제 14에 사용되는 함수
void sum_of_subsets(int, int, int, int, int*, int*);
int subset_promising(int, int, int, int, int*);

// 문제 17에 사용되는 함수
int estimate_subsets(int,int,int,int*);

int main() {
	printf("[문제 11] n-queens를 20번 수행할 떄의 평균 복잡도를 Monte Carlo를 이용해 구하기\n");
	int queenMonteSum=0, monteResult[20];
	srand((unsigned int)time(NULL));
	for(int i=0;i<20;i++){
		monteResult[i]=estimate_n_queens(8);
		queenMonteSum+=monteResult[i];
	}
	printf("--> n-queens 20번 수행 시 검사하는 노드 수의 평균: %d\n\n", queenMonteSum/20);

	
	printf("[문제 14] 2, 10, 13, 17, 22, 42에 대한 Sum-of-Subsets 해결\n");
	int arr[]={0,2,10,13,17,22,42}, n=7, total=0;
	for(int i=1;i<n;i++)
		total+=arr[i];
	int ans[n], W=52;
	sum_of_subsets(0, 0, total, W, arr, ans); // 함수 안에서 각 단계별 수행 로그 출력.
	printf("\n");
	
	
	printf("[문제 17] sum-of-subsets를 20번 수행한 평균 복잡도를 Monte Carlo를 이용해 구하기\n");
	int subsetMonteSum=0;
	for(int i=0;i<20;i++){
//		printf("%d번째 실행: ", i+1);
		int est=estimate_subsets(n, total, W, arr);
//		printf("탐색횟수: %d\n", est);
		subsetMonteSum+=est;
	}
	printf("--> sum-of-subsets 20번 수행 시 검사하는 노드 수의 평균: %d\n", subsetMonteSum/20);
}

// n-queens에서 현재 노드가 유망한지 검사하는 함수
// i: 현재 탐색중인 행 번호, col: 현재까지의 탐색 결과가 저장된 배열
int queen_promising(int i, int* col){
	int k=1, chk=1;
	
	while(k<i && chk){
		if(*(col+i)==*(col+k) || (*(col+i)-*(col+k))==i-k)
			return chk=0;
		k++;
	}
	return chk;
}

// n-queens 알고리즘에 대한 Monte Carlo 분석을 수행하는 함수
// n: 체스판 크기
int estimate_n_queens(int n){
	int m=1, mprod=1; // m: 자식 노드 개수, mprod: 현재 행에서의 예상 이웃노드 개수
	int numnodes=1, i=0; // numnodes: 전체 예상 탐색 노드 개수, i: 현재 탐색중인 행 번호
	int col[n+1]; // 현재까지 탐색한 결과가 저장된 배열. (ex. col[1]=1번째 행은 몇 번쨰 열에 퀸을 놓으면 되는지)
	int promisingChild, j; // promisingChild: 유망한 자식 노드에 대한 집합을 bitmask 형태로 표현. j: 인덱스
	
	while(m!=0 && i!=n){
		mprod*=m;
		numnodes+=(mprod*n);
		i++; // 행 번호 증가
		m=0;
		promisingChild=0;
		for(j=1; j<=n;j++){ // 열을 1개씩 증가시켜가며 탐색
			col[i]=j;
			if(queen_promising(i, col)){ // 유망한 노드라면
				m++; // 유망한 노드 개수 증가
				promisingChild|=(1<<j); // 유망한 자식 집합에 j열을 넣음
			}
		}
		if(m!=0){ // 유망한 노드가 1개라도 있다면
			// 유망한 자식노드가 선택될 때까지 반복적으로 랜덤픽 진행.
			while(1){
				j=((int)rand()%n)+1;
				if(promisingChild&(1<<j))
					break;
			}
			col[i]=j; // 임의로 지정한 j번째 열을 i행의 탐색 결과로 넣음
		}
	}
	return numnodes;
}

// sum of subsets를 수행하는 함수
// i: 탐색하는 행의 번호 (0=루트), weight: 현재까지 얻은 이익의 총 합
// total: 앞으로 남은 데이터들을 모두 더할 때 얻을 수 있는 이익의 최대치
// W: 탐색 목표값, arr: 입력 데이터가 들어있는 배열
// include: 현재 노드값을 포함하는지 여부. 포함할 경우 해당 인덱스의 데이터가 들어가고, 포함하지 않을 경우 0이 들어감.
void sum_of_subsets(int i, int weight, int total, int W, int* arr, int* include){
	if(subset_promising(i, weight, total, W, arr)){
		if(weight==W){ // W값과 동일한 덧셈의 조합을 찾은 경우 출력하는 부분
			for(int idx=1;idx<=i;idx++){
				int cur=*(include+idx);
				if(cur){
					printf("%d",cur);
					
					if(idx!=i)
						printf(" + ");
					else printf("\n");
				}
			}
			
		}
		else{ // W값과 동일하지 않은 경우
			// 현재 노드 값을 포함하는 자식 노드를 탐색
			include[i+1]=arr[i+1];
			sum_of_subsets(i+1, weight+arr[i+1], total-arr[i+1], W, arr, include);
			
			// 현재 노드 값을 포함하지 않는 자식 노드들을 탐색
			include[i+1]=0;
			sum_of_subsets(i+1, weight, total-arr[i+1], W, arr, include);
		}
	}
}

// 현재 노드가 유망한지 검색
// i: 유망 여부를 판단하려는 입력 데이터의 인덱스 번호, weight: 현재 노드에 포함된 가치의 합
// total: 앞으로 남은 노드들을 모두 더할 때 얻을 수 있는 최대의 가치
// W: 탐색 목표값, arr: 입력 데이터가 들어있는 배열
int subset_promising(int i, int weight, int total, int W, int* arr){
	return (weight+total>=W)&&(weight==W||weight+*(arr+i+1)<=W);
}

// sum of subset에 대한 Monte Calro 알고리즘
// n: 데이터 개수, total: 모든 데이터의 합, W: 찾으려는 합, arr: 데이터가 들어있는 배열
int estimate_subsets(int n, int total, int W, int* arr){
	int m=1, mprod=1; // m: 현재 노드의 자식 중 유망한 노드 개수, mprod: 현재 레벨에서 유망할 것으로 예상되는 노드 수
	int numnodes=1, i=0; // numnodes: 예상되는 탐색 노드 개수, i: 탐색하려는 자식 노드의 인덱스
	int remain_tot=total; // remain_tot: 앞으로 얻을 수 있는 최대의 이익
	int tree[n+1]; // 현재 데이터까지의 탐색 결과 (얻은 이익의 총 합)
	int promisingChild, j; // promisingChild: 유망한 자식 노드에 대한 집합을 bitmask 형태로 표현. j: 인덱스
	tree[0]=0;
	while(m!=0&&i!=n){
		mprod*=m;
		numnodes+=(mprod*2);
		i++; // 자식 노드의 인덱스로 이동.
		m=0; // 유망한 자식 노드 갯수 초기화
		promisingChild=0; // 유망한 자식 노드의 집합을 공집합으로 초기화
		for(j=1; j<=2;j++){ // j==1 : 현재 레벨의 데이터 포함. j==2 : 현재 레벨의 데이터 미포함.
			tree[i]=tree[i-1]+arr[i]*(2-j);
			if(subset_promising(i, tree[i], remain_tot-arr[i]*(j-1), W, arr)){
				m++;
				promisingChild|=(1<<j);
			}
		}
		if(m!=0){ // 유망한 자식 노드가 있다면
			// 유망한 자식노드가 선택될 때까지 반복적으로 랜덤픽 진행.
			while(1){
				j=((int)rand()%2)+1;
				if(promisingChild&(1<<j))
					break;
			}
			remain_tot-=arr[i]; // 현재 데이터를 포함하지 않는 경우 이론상 최대 이익의 값을 감소시킴.
			tree[i]=tree[i-1]+arr[i]*(2-j);
		}
	}
//	printf("%2d | ", tree[i]);
	return numnodes;
}
