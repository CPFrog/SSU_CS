#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 7

struct _node{
	char data[MAX];
	double p;
	struct _node* left;
	struct _node* right;
};
typedef struct _node* nodePtr;

int a[MAX][MAX]; // 탐색 비용이 저장되는 배열
int r[MAX][MAX]; // 최소한의 탐색 비용이 드는 노드를 저장하는 배열.
nodePtr root=NULL; // 트리의 root node를 가리키는 포인터
nodePtr node[MAX]; // 노드의 주소값을 가지고 있는 포인터 배열

char datas[6][5]={"case","else","end","if","of","then"}; // 초기에 주어진 데이터
double probs[]={.05,.15,.05,.35,.05,.35}; // 각 데이터들의 확률

void optNode(int);
void search(nodePtr, nodePtr);
nodePtr tree(int, int);
void inOrderSearch(nodePtr);


int main(){
	int dataNum=6;
	for(int i=1;i<=dataNum+1;i++){
		nodePtr newNode=(nodePtr)malloc(sizeof(struct _node)); // 새로운 노드가 저장될 공간을 동적할당
		
		strcpy(newNode->data,datas[i-1]); // 새로운 노드에 데이터 복사
		newNode->p=probs[i-1]; // 새로운 노드에 확률 복사
		
		newNode->left=NULL;
		newNode->right=NULL;
		node[i]=newNode; // 노드 포인터 배열에 방금 생성한 새로운 노드를 저장.
		
		for(int j=0;j<=dataNum;j++){
			a[i][j]=MAX;
		}
	}
	
	optNode(dataNum);
	
	root=tree(1, dataNum);
	
	// 생성된 트리가 어떤 형태인지 알아보기 위해 중위순회 결과 출력.
	printf("트리를 Inorder로 탐색한 결과 : ");
	inOrderSearch(root);
	printf("탐색 완료\n");
	
	return 0;
}

// 평균 탐색 시간이 최소가 되는 노드를 찾는 함수
// n: 데이터 개수
void optNode(int n){
	for(int i=1;i<=n;i++){
		a[i][i-1]=0;
		a[i][i]=node[i]->p;
		r[i][i]=i;
		r[i][i-1]=0;
	}
	
	a[n+1][n]=0;
	r[n+1][n]=0;
	
	for(int diag=1;diag<=n-1;diag++){
		for(int i=1;i<=n-diag;i++){
			float sum=0;
			int j=i+diag;
			for(int k=i;k<=j;k++)
				sum+=node[k]->p; // 각각의 탐색시간 저장.
			for(int k=i;k<=j;k++){
				// 탐색 시간이 최소가 되는 노드를 배열 r에 저장
				if(a[i][j]>a[i][k-1]+a[k+1][j]+sum){
					a[i][j]=a[i][k-1]+a[k+1][j]+sum;
					r[i][j]=k;
				}
			}
		}
	}
}

// 루트 노드를 중심으로 트리 생성
// i: 트리를 생성할 배열의 시작 인덱스, j: 트리를 생성할 배열의 끝 인덱스
nodePtr tree(int i, int j){
	nodePtr p;
	
	int k=r[i][j];
	if(k==0) return NULL;
	else{
		p=node[k];
		p->left=tree(i,k-1);
		p->right=tree(k+1,j);
		return p;
	}
}

// 트리 중위순회(Inorder traversal) 함수
// cur: 현 위치의 노드를 가리키는 포인터
void inOrderSearch(nodePtr cur){
	if(cur!=NULL){
		inOrderSearch(cur->left);
		printf("%s -> ", cur->data);
		inOrderSearch(cur->right);
	}
}
