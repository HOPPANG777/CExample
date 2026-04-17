// 실행: 콘솔에서 실행 후, 스페이스바로 할당 / ESC로 종료

// scanf를 사용하기 위한 매크로입니다.
#define _CRT_SECURE_NO_WARNINGS

// C 표준 컴파일 헤더입니다.
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <conio.h>   // _kbhit, _getch
#include <windows.h> // Sleep

// C++ 컴파일러에서 감지되는 경고 메세지를 무시합니다. 
// 현재 이 프로젝트는 C 언어 연습을 위한 프로젝트이므로 C++는 사용하지 않습니다.
#pragma warning( disable : 4096 )

typedef struct
{
    void* ptr;
    size_t size;
} Block;

int main()
{
    const size_t initial_size = 1024; // 시작 1 KiB
    size_t next_size = initial_size;
    size_t capacity = 16;
    size_t count = 0;

    Block* blocks = (Block*)malloc(sizeof(Block) * capacity);
    if (!blocks)
    {
        fprintf(stderr, "메타데이터 배열 할당 실패\n");
        return 1;
    }

    printf("[조작법] 스페이스: 새 블록 할당(이전보다 큰 크기),  ESC: 종료\n");
    printf("초기 다음 크기: %zu bytes\n\n", next_size);

    void* prev = NULL;

    for (;;)
    {
        if (_kbhit())
        {
            int ch = _getch();

            if (ch == 27)
            {
                break;
            }

            if (ch == ' ')
            {
                // 필요 시 메타데이터 배열 확장
                if (count == capacity)
                {
                    size_t newcap = capacity * 2;
                    Block* nb = (Block*)realloc(blocks, sizeof(Block) * newcap);
                    if (!nb)
                    {
                        fprintf(stderr, "메타데이터 확장 실패\n");
                        break;
                    }
                    blocks = nb;
                    capacity = newcap;
                }

                // 새 블록 할당
                void* p = malloc(next_size);
                if (!p)
                {
                    fprintf(stderr, "malloc(%zu) 실패\n", next_size);
                    break;
                }

                blocks[count].ptr = p;
                blocks[count].size = next_size;
                count++;

                // 출력
                long long diff = 0;
                if (prev)
                {
                    diff = (long long)((char*)p - (char*)prev);
                }

                printf("%2zu번째 할당: %p (크기: %zu bytes)\n", count, p, next_size);
                if (prev)
                {
                    printf("    이전 블록(%p)과의 거리: %+lld bytes (0x%+llX)\n",
                        prev, diff, (unsigned long long)diff);
                }
                else
                {
                    printf("    이전 블록 없음\n");
                }
                puts("");

                prev = p;

                // 다음 크기 결정: 이전보다 커야 하므로 2배씩 증가 (원하면 바꾸셈, 감소로 해도 되고...)
                if (next_size <= SIZE_MAX / 2)
                {
                    next_size *= 2;
                }
                else
                {
                    // 오버플로 방지
                    next_size += 1024;
                }
                printf("다음 할당 예정 크기: %zu bytes\n\n", next_size);
            }
        }

        // CPU 점유율 방지용 짧은 휴식
        Sleep(10);
    }

    // 정리: 할당한 블록들 해제
    for (size_t i = 0; i < count; ++i)
    {
        free(blocks[i].ptr);
    }
    free(blocks);

    puts("\n종료합니다.");
    return 0;
}