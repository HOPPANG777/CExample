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

void DataTypeMinMax()
{
    printf("char        : %zu bytes, range = %d ~ %d\n", sizeof(char), CHAR_MIN, CHAR_MAX);
    printf("short       : %zu bytes, range = %d ~ %d\n", sizeof(short), SHRT_MIN, SHRT_MAX);
    printf("int         : %zu bytes, range = %d ~ %d\n", sizeof(int), INT_MIN, INT_MAX);
    printf("long        : %zu bytes, range = %ld ~ %ld\n", sizeof(long), LONG_MIN, LONG_MAX);
    printf("long long   : %zu bytes, range = %lli ~ %lli\n", sizeof(long long), LLONG_MIN, LLONG_MAX);
    printf("float       : %zu bytes, range = %.10e ~ %.10e\n", sizeof(float), FLT_MIN, FLT_MAX);
    printf("double      : %zu bytes, range = %.10e ~ %.10e\n", sizeof(double), DBL_MIN, DBL_MAX);
    printf("long double : %zu bytes, range = %.20Le ~ %.20Le\n", sizeof(long double), LDBL_MIN, LDBL_MAX);
}