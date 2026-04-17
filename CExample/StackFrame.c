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

void nested(int depth)
{
    int x = depth;
    printf("depth=%d, &x=%p\n", depth, (void*)&x);
    if (depth < 3) nested(depth + 1);
}

int StackFrame()
{
    nested(1);
}