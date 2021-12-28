// 코드 작성 환경 정보
// OS : macOS (Big Sur) 11.5.2
// IDE 환경 : Xcode 12.5.1

#include <stdio.h>
#include <stdlib.h>

int main(){
    int num; //배열의 원소 갯수
    scanf("%d", &num);
    
    int *arr=(int*)malloc(sizeof(int)*num); //정렬된 데이터들을 읽어들일 배열을 동적할당.
    
    for(int i=0;i<num;i++)
        scanf("%d",arr+i);
    
    int search; //찾으려는 데이터의 값
    scanf("%d",&search);
    
    int left=0, right=num-1; // 왼쪽 끝의 인덱스 번호, 오른쪽 끝의 인덱스 번호.
    int mid=0; //중앙 인덱스 번호
    while(left<right){
        mid=(left+right)/2;
        // arr의 주소+mid*sizeof(int)의 주소에 있는 값이 찾으려는 값과 비교
        if(*(arr+mid)==search) //같을 경우
            break;
        else if(*(arr+mid)<search) //다를 경우
            left=mid+1;
        else
            right=mid;
    }
    
    if(left<right)
        printf("%d\n",mid+1); //문제 명세상 인덱스의 시작을 1로 두는 것 같아 (인덱스번호)+1 출력.
    else
        printf("실패\n");
    
    return 0;
}
