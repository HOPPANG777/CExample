// scanf를 사용하기 위한 매크로입니다.
#define _CRT_SECURE_NO_WARNINGS

// C 표준 컴파일 헤더입니다.
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

// C++ 컴파일러에서 감지되는 경고 메세지를 무시합니다. 
// 현재 이 프로젝트는 C 언어 연습을 위한 프로젝트이므로 C++는 사용하지 않습니다.
#pragma warning( disable : 4096 )

void print_bits(unsigned int value, int bits)
{
    // 높은 비트부터 하나씩 출력
    for (int i = bits - 1; i >= 0; i--) {
        unsigned int mask = 1u << i;
        putchar((value & mask) ? '1' : '0');
        if (i % 4 == 0) putchar(' ');
    }
}

int main()
{
    unsigned char x = 250;   // 0b11111010
    unsigned char y = 10;    // 0b00001010
    unsigned char z = x + y; // int로 승격 수행

    unsigned int result = x + y; // 260

    printf("x = %3u : ", x); print_bits(x, 8); printf("\n");
    printf("y = %3u : ", y); print_bits(y, 8); printf("\n");

    printf("\n--- 연산 ---\n");
    printf("x + y = %u : ", result); print_bits(result, 9); printf("(9비트)\n");

    printf("\n--- 저장 ---\n");
    printf("z = %3u : ", z); print_bits(z, 8); printf("(8비트)\n");

    printf("\n정리:\n");
    printf("260(0b1 0000 0100) → 상위 1비트 잘리고 0b0000 0100만 남음 → z = 4\n");

    return 0;
}

/*
    unsigned char x = 250;
    unsigned char y = 10;
    unsigned char z = x + y;

    printf("x + y = %u (z=%u)\n", x + y, z);
*/