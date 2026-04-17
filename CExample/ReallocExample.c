// scanf를 사용하기 위한 매크로입니다.
#define _CRT_SECURE_NO_WARNINGS

// C 표준 컴파일 헤더입니다.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C++ 컴파일러에서 감지되는 경고 메세지를 무시합니다. 
// 현재 이 프로젝트는 C 언어 연습을 위한 프로젝트이므로 C++는 사용하지 않습니다.
#pragma warning( disable : 4096 )

int main() 
{
    size_t n; // 원소 개수

    // 원소 개수 입력
    printf("원소 개수 n을 입력하세요: ");
    if (scanf("%zu", &n) != 1) 
    {
        fprintf(stderr, "입력 오류\n");
        return 1;
    }

    // 메모리 공간 할당 및 0으로 초기화
    int* arr = calloc(n, sizeof(int));  
    if (arr == NULL) 
    {
        fprintf(stderr, "메모리 할당 실패\n");
        return 1;
    }

    // calloc 덕분에 초기값은 모두 0
    printf("초기 값들:\n");
    for (size_t i = 0; i < n; ++i) 
    {
        // 해당 열에 가지고 있는 값들을 모두 출력 현재 상태에선 
        // 따로 값을 넣지 않았기 때문에 모두 0으로 출력됨.
        printf("arr[%zu] = %d\n", i, arr[i]); 
    }

    // 값 채우기
    for (size_t i = 0; i < n; ++i) 
    {
        // 행 별로 i * i만큼 채워짐.
        arr[i] = (int)(i * i);  
    }

    printf("수정 후 값들:\n");
    for (size_t i = 0; i < n; ++i) 
    {
        // 최종적으로 해당 열과 행에 추가된 값들을 모두 출력한다.
        printf("arr[%zu] = %d\n", i, arr[i]);  
    }

    free(arr);  // 해당 메모리 공간 해제.
    return 0;
}