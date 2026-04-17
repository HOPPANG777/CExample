#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// 전역(=정적) 영역에 있는 변수
char global_buf[10] = { 0 };

// 메모리 영역 판별 함수
const char* region_of(const void* ptr,
    const void* stack_sample,
    const void* heap_start,
    const void* heap_end)
{
    // 스택(stack)
    if ((char*)ptr >= (char*)stack_sample - 20000 &&
        (char*)ptr <= (char*)stack_sample + 20000)
        return "스택(Stack)";

    // 힙(heap)
    if ((char*)ptr >= (char*)heap_start &&
        (char*)ptr <= (char*)heap_end)
        return "힙(Heap)";

    // 전역/정적 영역(Global/static)
    if ((char*)ptr >= (char*)global_buf &&
        (char*)ptr <= (char*)global_buf + sizeof(global_buf))
        return "전역/정적(Global/Static)";

    // 함수 포인터 등: 보통 텍스트 영역(code) 또는 OS 영역
    return "코드/텍스트(Text Segment) 또는 기타 영역";
}

// 함수 자체(코드)의 주소를 확인하기 위한 더미 함수
void dummy_function(void)
{
    int x = 0;
    x++;  // 아무 의미 없는 코드
}

int main(void)
{
    char buf[100];           // 스택에 있는 배열
    char* p = buf;           // 포인터도 스택
    char stack_sample;       // 스택 기준 비교용
    char* heap_buf = malloc(50);   // 힙
    char* heap_end = heap_buf + 50;

    printf("문자열 입력 (예: 123,45): ");
    fgets(buf, sizeof(buf), stdin);

    printf("\n============================\n");
    printf("  문자열(buf) 정보\n");
    printf("============================\n");

    printf("입력된 문자열: %s", buf);
    printf("buf 배열 시작 주소(&buf[0]): %p → %s\n",
        (void*)&buf[0],
        region_of(&buf[0], &stack_sample, heap_buf, heap_end));

    printf("buf 라는 이름 자체(buf): %p → %s\n",
        (void*)buf,
        region_of(buf, &stack_sample, heap_buf, heap_end));

    printf("포인터 p가 가리키는 주소: %p → %s\n",
        (void*)p,
        region_of(p, &stack_sample, heap_buf, heap_end));

    printf("전역 global_buf 주소: %p → %s\n",
        (void*)global_buf,
        region_of(global_buf, &stack_sample, heap_buf, heap_end));


    printf("\n============================\n");
    printf("  문자열 각 문자 주소\n");
    printf("============================\n");

    int i = 0;
    while (buf[i] != '\0') {
        printf("buf[%d] = '%c' (ASCII %d), 주소 = %p → %s\n",
            i,
            buf[i],
            (unsigned char)buf[i],
            (void*)&buf[i],
            region_of(&buf[i], &stack_sample, heap_buf, heap_end));
        i++;
    }

    printf("buf[%d] = '\\0', 주소 = %p → %s\n",
        i,
        (void*)&buf[i],
        region_of(&buf[i], &stack_sample, heap_buf, heap_end));


    printf("\n============================\n");
    printf("  함수(main, region_of, dummy_function) 정보\n");
    printf("============================\n");

    printf("main 함수 주소:           %p → %s\n",
        (void*)main,
        region_of(main, &stack_sample, heap_buf, heap_end));

    printf("region_of 함수 주소:     %p → %s\n",
        (void*)region_of,
        region_of(region_of, &stack_sample, heap_buf, heap_end));

    printf("dummy_function 함수 주소: %p → %s\n",
        (void*)dummy_function,
        region_of(dummy_function, &stack_sample, heap_buf, heap_end));


    printf("\n============================\n");
    printf("  힙(heap) 메모리 정보\n");
    printf("============================\n");

    printf("heap_buf 시작 주소: %p → %s\n",
        (void*)heap_buf,
        region_of(heap_buf, &stack_sample, heap_buf, heap_end));

    printf("heap_buf 끝 주소:   %p → %s\n",
        (void*)heap_end,
        region_of(heap_end, &stack_sample, heap_buf, heap_end));

    free(heap_buf);

    return 0;
}
