#include "UBTest.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * File: simple_overflow.c
 * Compiler: MSVC 2026 (x64) - Release Mode
 */

void UBTest()
{
    // 변수 배치
    // 컴파일러는 스택 메모리에 이 두 변수를 나란히 붙여서 배치할 가능성이 높다
    volatile int is_admin = 0;  // 0: 일반 유저, 1: 관리자
    int buffer[4] = { 10, 20, 30, 40 }; // 정수 4개짜리 배열

    // 문제의 반복문
    // 배열의 값을 모두 99로 초기화하려고 합니다.
    for (int i = 0; i <= 4; i++)
    {
        buffer[i] = 99;
    }

    // 결과 확인
    if (is_admin != 0)
    {
        printf("일반 유저가 관리자 권한을 획득했습니다.\n");
        printf("is_admin 값: %d\n", is_admin);
    }
    else
    {
        printf("정상: 일반 유저입니다.\n");
    }

    printf("\n");

    system("pause");
}
