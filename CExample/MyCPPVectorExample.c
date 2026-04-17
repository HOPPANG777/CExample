// scanf를 사용하기 위한 매크로입니다.
#define _CRT_SECURE_NO_WARNINGS

// C 표준 컴파일 헤더입니다.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C++ 컴파일러에서 감지되는 경고 메세지를 무시합니다. 
// 현재 이 프로젝트는 C 언어 연습을 위한 프로젝트이므로 C++는 사용하지 않습니다.
#pragma warning( disable : 4096 )

typedef struct 
{
    int* data;
    size_t  size;     // 실제 원소 개수
    size_t  capacity; // 할당된 공간 (int 개수)
} IntVec;

void intvec_init(IntVec* v) 
{
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

void intvec_push_back(IntVec* v, int value) 
{
    if (v->size == v->capacity) 
    {
        // 공간이 꽉 찼으므로 늘려야 함
        size_t new_cap = (v->capacity == 0) ? 4 : v->capacity * 2;

        void* tmp = realloc(v->data, new_cap * sizeof(int));
        if (tmp == NULL) 
        {
            // 실패: v->data는 여전히 유효.
            fprintf(stderr, "realloc 실패\n");
            // 여기서는 단순히 프로그램 종료
            free(v->data);
            exit(1);
        }

        v->data = tmp;
        v->capacity = new_cap;
    }

    v->data[v->size++] = value;
}

void intvec_free(IntVec* v) 
{
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

int main(void) 
{
    IntVec v;
    intvec_init(&v);

    for (int i = 0; i < 10; ++i) 
    {
        intvec_push_back(&v, i * 10);
    }

    for (size_t i = 0; i < v.size; ++i) 
    {
        printf("v[%zu] = %d\n", i, v.data[i]);
    }

    intvec_free(&v);
    return 0;
}