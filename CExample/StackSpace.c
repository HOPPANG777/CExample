
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <malloc.h>
#include <intrin.h>

// UI 도우미
static void set_color(WORD attr) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h && h != INVALID_HANDLE_VALUE) SetConsoleTextAttribute(h, attr);
}
static void divider(const char* title) {
    set_color(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    printf("──────────────────────────────────────────────────────────────\n");
    if (title && *title) printf("%s\n", title);
    printf("──────────────────────────────────────────────────────────────\n");
    set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
static void kvp(const char* k, const void* v) {
    printf("  %-20s : %p\n", k, v);
}
static void kvp_i64(const char* k, long long v) {
    printf("  %-20s : %+lld (0x%+llX)\n", k, v, (unsigned long long)v);
}
static long long ptr_diff(const void* a, const void* b) {
    return (long long)((const char*)a - (const char*)b);
}

// HEAP 상태
typedef struct { void* ptr; size_t size; } HeapBlk;

typedef struct {
    size_t cap, cnt;
    HeapBlk* arr;
    void* prev_ptr;
    size_t  next_size;
} HeapState;

static int heap_init(HeapState* H, size_t initial_size) {
    H->cap = 16; H->cnt = 0;
    H->arr = (HeapBlk*)malloc(sizeof(HeapBlk) * H->cap);
    H->prev_ptr = NULL;
    H->next_size = initial_size;
    return H->arr != NULL;
}
static void heap_free_all(HeapState* H) {
    for (size_t i = 0; i < H->cnt; ++i) free(H->arr[i].ptr);
    free(H->arr);
    H->arr = NULL;
}
static void heap_alloc_more(HeapState* H) {
    if (H->cnt == H->cap) {
        size_t ncap = H->cap * 2;
        HeapBlk* nb = (HeapBlk*)realloc(H->arr, sizeof(HeapBlk) * ncap);
        if (!nb) { fprintf(stderr, "메타데이터 확장 실패\n"); return; }
        H->arr = nb; H->cap = ncap;
    }
    void* p = malloc(H->next_size);
    divider("[HEAP] 새 블록 할당");
    if (!p) {
        set_color(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("  malloc(%zu) 실패\n\n", H->next_size);
        set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }
    H->arr[H->cnt].ptr = p;
    H->arr[H->cnt].size = H->next_size;
    H->cnt++;

    printf("  #%02zu  size=%zu bytes\n", H->cnt, H->next_size);
    kvp("ptr", p);
    if (H->prev_ptr) {
        kvp("prev", H->prev_ptr);
        kvp_i64("Δ(ptr - prev)", ptr_diff(p, H->prev_ptr));
    }
    else {
        printf("  %-20s : (없음)\n", "prev");
    }
    printf("\n");

    H->prev_ptr = p;
    if (H->next_size <= SIZE_MAX / 2) H->next_size *= 2; else H->next_size += 1024;
}

// ──────────────────────────────────────────────────────────────
// STACK 상태 (런타임 스냅샷)
//  - 지역 변수(marker)
//  - alloca 블록 시작(새로 내려간 스택 최저점)
//  - 반환주소 슬롯(스택에 저장된 caller return address 위치)
//  - 반환주소 값
//  - RSP/RIP (컨텍스트 캡처)
// ──────────────────────────────────────────────────────────────
typedef struct {
    void* marker_addr;      // 현재 프레임 기준 지역변수 주소
    void* alloca_ptr;       // _alloca() 반환 포인터
    void* retaddr_slot;     // _AddressOfReturnAddress()
    void* retaddr_value;    // _ReturnAddress()
    void* rsp;              // 캡처한 스택 포인터
    void* rip;              // 캡처한 명령 포인터
    size_t bytes;           // 요청한 alloca 바이트
} StackSnap;

typedef struct {
    StackSnap prev;
    int       has_prev;
    size_t    next_bytes;
} StackState;

__declspec(noinline)
static void capture_stack_snapshot(StackState* S) {
    // 1) 프레임 기준 지역 변수(고정 오프셋)
    volatile char marker = 0x5A;
    void* marker_addr = (void*)&marker;

    // 2) 런타임 스택 공간 확보 (스택 아래로 확장)
    void* ap = _alloca(S->next_bytes);
    ((volatile unsigned char*)ap)[0] = 0xA5; // 실제 접근(커밋) 유도

    // 3) 반환주소 슬롯과 값
    void* ret_slot = _AddressOfReturnAddress();
    void* ret_value = _ReturnAddress();

    // 4) 컨텍스트 캡처 (RSP/RIP)
    CONTEXT ctx; RtlCaptureContext(&ctx);

    // 스냅샷 구성
    StackSnap cur = {
        .marker_addr = marker_addr,
        .alloca_ptr = ap,
        .retaddr_slot = ret_slot,
        .retaddr_value = ret_value,
        .rsp = (void*)ctx.Rsp,
        .rip = (void*)ctx.Rip,
        .bytes = S->next_bytes
    };

    // UI 출력
    divider("[STACK] 런타임 스냅샷 (alloca로 프레임 하강)");
    printf("  요청 크기(bytes)   : %zu\n\n", cur.bytes);

    set_color(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    puts("  ■ 절대 주소");
    set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    kvp("marker(&local)", cur.marker_addr);
    kvp("alloca_ptr", cur.alloca_ptr);
    kvp("retaddr_slot", cur.retaddr_slot);
    kvp("retaddr_value", cur.retaddr_value);
    kvp("RSP (captured)", cur.rsp);
    kvp("RIP (captured)", cur.rip);
    printf("\n");

    if (S->has_prev) {
        set_color(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        puts("  ■ 직전 스냅샷과 Δ거리");
        set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        kvp_i64("Δ marker", ptr_diff(cur.marker_addr, S->prev.marker_addr));
        kvp_i64("Δ alloca_ptr", ptr_diff(cur.alloca_ptr, S->prev.alloca_ptr));
        kvp_i64("Δ retaddr_slot", ptr_diff(cur.retaddr_slot, S->prev.retaddr_slot));
        kvp_i64("Δ RSP", ptr_diff(cur.rsp, S->prev.rsp));
        // RIP은 코드 위치 동일 함수 내라 보통 동일/근접
        kvp_i64("Δ RIP", ptr_diff(cur.rip, S->prev.rip));
        printf("\n");

        set_color(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        puts("  해설");
        set_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        puts("   - marker: 프레임 내 고정 오프셋 → 동일 주소일 수 있음(프레임 재사용).");
        puts("   - alloca_ptr: 스택을 즉시 더 낮은 주소로 당김 → 일반적으로 이전보다 더 작은 주소.");
        puts("   - retaddr_slot: 호출자의 복귀 주소가 적힌 슬롯 위치(스택 상 위치).");
        puts("   - RSP: 현재 스택 포인터. alloca로 하강하면 더 작은 값(하위 주소)로 이동.");
        puts("   - RIP: 현재 명령 포인터(해당 지점 코드 주소).");
        printf("\n");
    }
    else {
        printf("  (첫 스냅샷이므로 Δ 비교 없음)\n\n");
    }

    // 다음 비교를 위해 저장
    S->prev = cur;
    S->has_prev = 1;

    // 다음 요청 크기 (2배 증가; 필요시 선형으로 변경 가능)
    if (S->next_bytes <= SIZE_MAX / 2) S->next_bytes *= 2;
    else S->next_bytes += 4096;
}

// ──────────────────────────────────────────────────────────────
// 메인
// ──────────────────────────────────────────────────────────────
int main(void) {
    HeapState H;
    StackState S = { 0 };
    if (!heap_init(&H, 1024)) { fprintf(stderr, "heap 메타데이터 초기화 실패\n"); return 1; }
    S.next_bytes = 4096;

    divider("조작법");
    puts("  SPACE : 새 힙 블록 할당 (이전보다 큰 크기)");
    puts("  S     : 스택(allo ca) 스냅샷 캡처 및 비교");
    puts("  ESC   : 종료");
    printf("\n");

    for (;;) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 27) { // ESC
                break;
            }
            else if (ch == ' ') {
                heap_alloc_more(&H);
                printf("\n");
            }
            else if (ch == 's' || ch == 'S') {
                capture_stack_snapshot(&S);
                printf("\n");
            }
        }
        Sleep(10);
    }

    divider("해제");
    heap_free_all(&H);
    puts("  모든 힙 블록 해제 완료.\n");

    divider("종료");
    puts("  프로그램을 종료합니다.\n");
    return 0;
}
