// scanf를 사용하기 위한 매크로입니다.
#define _CRT_SECURE_NO_WARNINGS

// C 표준 컴파일 헤더입니다.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C++ 컴파일러에서 감지되는 경고 메세지를 무시합니다. 
// 현재 이 프로젝트는 C 언어 연습을 위한 프로젝트이므로 C++는 사용하지 않습니다.
#pragma warning( disable : 4096 )

#include "MyVector2.h"
#include "MyVectorExample.h"
#include "UBTest.h"
#include "BulletHellSpawnSystem.h"
#include "FunctionPointerExample1.h"

//int add(int a, int b)
//{
//    return a + b;
//}

//struct Data
//{
//    int first;
//    char second;
//};
//
//struct Test
//{
//    int data;
//    struct Test next;
//};

int main() 
{
    //int (*fp)(int, int);  // 선언

    //fp = add;      // 함수 이름 직접 대입
    //fp = &add;     // 명시적 주소 연산자 (동일)

    //int result1 = fp(3, 4);      // 직접 호출
    //int result2 = (*fp)(3, 4);   // 역참조 후 호출 (동일)

    //printf("%d %d\n", result1, result2);  // 7 7

    //FunctionPointerExampleTest();

    //struct Unknown *x;

    //struct Data d;

    //printf("%p\n", &d);
    //printf("%p\n", &d.first);
    //printf("%p\n", &d.second);

    int time, multiply = 0;

    for (int i = 1; i < 9; i++)
    {
        if (i % 2 != 0)
        {
            continue;
        }

        for (int j = 1; j < 9; j++)
        {
            printf("%d X %d = %d\n", i, j, i * j);
            
            if (j % i == 0)
            {
                break;
            }
        }

        printf("\n");
    }

    printf("\n");

    system("pause");

    return 0;
}

