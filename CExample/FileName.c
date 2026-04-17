#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 호출 규약별 함수 포인터 타입 정의
typedef int(__cdecl* CdeclFunc)(int, int);
typedef int(__stdcall* StdcallFunc)(int, int);
typedef void(__cdecl* Callback)(const char*);

// 기본 연산 함수들
int __cdecl add(int a, int b) { return a + b; }
int __cdecl sub(int a, int b) { return a - b; }
int __cdecl mul(int a, int b) { return a * b; }

// 함수 포인터를 반환하는 함수
CdeclFunc get_operation(char op) 
{
    switch (op) 
    {
    case '+': return add;
    case '-': return sub;
    case '*': return mul;
    default: return NULL;
    }
}

// vtable 시뮬레이션: 구조체 내 함수 포인터
typedef struct 
{
    void (*init)(void* self, int value);
    void (*process)(void* self);
    void (*destroy)(void* self);
    int data;
} Object;

void obj_init(void* self, int value) 
{
    Object* obj = (Object*)self;
    obj->data = value;
    printf("Initialized: %d\n", value);
}

void obj_process(void* self) 
{
    Object* obj = (Object*)self;
    obj->data *= 2;
    printf("Processed: %d\n", obj->data);
}

void obj_destroy(void* self) 
{
    Object* obj = (Object*)self;
    printf("Destroyed: %d\n", obj->data);
    obj->data = 0;
}

Object create_object() 
{
    Object obj;
    obj.init = obj_init;
    obj.process = obj_process;
    obj.destroy = obj_destroy;
    obj.data = 0;
    return obj;
}

// 함수 포인터 배열을 이용한 상태 머신
typedef enum { STATE_IDLE, STATE_RUNNING, STATE_PAUSED, STATE_MAX } State;

void state_idle() { printf("[IDLE]\n"); }
void state_running() { printf("[RUNNING]\n"); }
void state_paused() { printf("[PAUSED]\n"); }

typedef void (*StateHandler)();
StateHandler state_handlers[STATE_MAX] = 
{
    state_idle,
    state_running,
    state_paused
};

// 고차 함수: 함수를 인자로 받아 다른 함수 생성
typedef int (*Filter)(int);

int is_even(int x) { return x % 2 == 0; }
int is_positive(int x) { return x > 0; }

void filter_array(int* arr, int size, Filter predicate, int* out, int* out_size) 
{
    *out_size = 0;
    for (int i = 0; i < size; i++) 
    {
        if (predicate(arr[i])) 
        {
            out[(*out_size)++] = arr[i];
        }
    }
}

// qsort 스타일 비교 함수 포인터
int __cdecl compare_asc(const void* a, const void* b) 
{
    return (*(int*)a - *(int*)b);
}

int __cdecl compare_desc(const void* a, const void* b) 
{
    return (*(int*)b - *(int*)a);
}

// Windows API 스타일 콜백
typedef struct 
{
    int id;
    const char* name;
} Item;

BOOL CALLBACK enum_callback(void* context, Item* item) 
{
    int* count = (int*)context;
    printf("  [%d] %s\n", item->id, item->name);
    (*count)++;
    return TRUE;
}

void enumerate_items(Item* items, int count, BOOL(CALLBACK* callback)(void*, Item*), void* context) 
{
    for (int i = 0; i < count; i++) 
    {
        if (!callback(context, &items[i])) break;
    }
}

// 함수 포인터 체이닝
typedef int (*Transform)(int);

int chain_transforms(int value, Transform* transforms, int count) 
{
    int result = value;
    for (int i = 0; i < count; i++) 
    {
        result = transforms[i](result);
    }
    return result;
}

int add_10(int x) { return x + 10; }
int mul_2(int x) { return x * 2; }
int sub_5(int x) { return x - 5; }

// 함수 포인터 테이블 (점프 테이블)
#define CMD_MAX 4
typedef void (*CommandFunc)(int);

void cmd_print(int val) { printf("Print: %d\n", val); }
void cmd_square(int val) { printf("Square: %d\n", val * val); }
void cmd_negate(int val) { printf("Negate: %d\n", -val); }
void cmd_invalid(int val) { printf("Invalid command\n"); }

CommandFunc command_table[CMD_MAX] = 
{
    cmd_print,
    cmd_square,
    cmd_negate,
    cmd_invalid
};

// 함수 포인터 캐스팅 (주의: 타입 안전하지 않음)
typedef void (*GenericFunc)();

void specific_func(int a, int b, int c) 
{
    printf("Called with: %d, %d, %d\n", a, b, c);
}

void call_generic(GenericFunc func) 
{
    // 실제로는 타입이 맞아야 함 - 여기서는 시연용
    ((void(*)(int, int, int))func)(1, 2, 3);
}

// 재귀 함수 포인터
typedef int (*RecursiveFunc)(int, RecursiveFunc);

int factorial_impl(int n, RecursiveFunc self) 
{
    return (n <= 1) ? 1 : n * self(n - 1, self);
}

// 런타임 함수 선택 (전략 패턴)
typedef struct 
{
    const char* name;
    int (*execute)(int*, int);
} Strategy;

int sum_strategy(int* arr, int size) 
{
    int sum = 0;
    for (int i = 0; i < size; i++) sum += arr[i];
    return sum;
}

int product_strategy(int* arr, int size) 
{
    int prod = 1;
    for (int i = 0; i < size; i++) prod *= arr[i];
    return prod;
}

int max_strategy(int* arr, int size) 
{
    int max = arr[0];
    for (int i = 1; i < size; i++)
        if (arr[i] > max) max = arr[i];
    return max;
}

Strategy strategies[] = 
{
    {"Sum", sum_strategy},
    {"Product", product_strategy},
    {"Max", max_strategy}
};

int main() 
{
    printf("=== Function Pointer Showcase ===\n\n");

    // 함수 포인터 할당 및 호출
    printf("1. Basic Function Pointer:\n");
    CdeclFunc op = add;
    printf("  %d + %d = %d\n", 5, 3, op(5, 3));
    op = mul;
    printf("  %d * %d = %d\n\n", 5, 3, op(5, 3));

    // 함수 포인터 반환
    printf("2. Returning Function Pointer:\n");
    CdeclFunc selected = get_operation('*');
    printf("  Selected operation: %d\n\n", selected(7, 6));

    // vtable 시뮬레이션
    printf("3. VTable Simulation:\n");
    Object obj = create_object();
    obj.init(&obj, 42);
    obj.process(&obj);
    obj.destroy(&obj);
    printf("\n");

    // 상태 머신
    printf("4. State Machine:\n");
    for (int state = STATE_IDLE; state < STATE_MAX; state++) 
    {
        printf("  State %d: ", state);
        state_handlers[state]();
    }
    printf("\n");

    // 고차 함수 (filter)
    printf("5. Higher-Order Function (Filter):\n");
    int numbers[] = { -2, 3, -5, 8, 0, 12, -7 };
    int filtered[7];
    int filtered_count;

    filter_array(numbers, 7, is_even, filtered, &filtered_count);
    printf("  Evens: ");
    for (int i = 0; i < filtered_count; i++) printf("%d ", filtered[i]);

    filter_array(numbers, 7, is_positive, filtered, &filtered_count);
    printf("\n  Positives: ");
    for (int i = 0; i < filtered_count; i++) printf("%d ", filtered[i]);
    printf("\n\n");

    // qsort with function pointer
    printf("6. Sorting with Comparator:\n");
    int data[] = { 5, 2, 9, 1, 7 };
    qsort(data, 5, sizeof(int), compare_asc);
    printf("  Ascending: ");
    for (int i = 0; i < 5; i++) printf("%d ", data[i]);

    qsort(data, 5, sizeof(int), compare_desc);
    printf("\n  Descending: ");
    for (int i = 0; i < 5; i++) printf("%d ", data[i]);
    printf("\n\n");

    // Windows API 스타일 콜백
    printf("7. Windows-Style Callback:\n");
    Item items[] = 
    {
        {1, "Engine"},
        {2, "Renderer"},
        {3, "Physics"}
    };
    int item_count = 0;
    enumerate_items(items, 3, enum_callback, &item_count);
    printf("  Total: %d items\n\n", item_count);

    // 함수 체이닝
    printf("8. Function Chaining:\n");
    Transform transforms[] = { add_10, mul_2, sub_5 };
    int result = chain_transforms(5, transforms, 3);
    printf("  (5 + 10) * 2 - 5 = %d\n\n", result);

    // 점프 테이블
    printf("9. Jump Table:\n");
    for (int cmd = 0; cmd < CMD_MAX; cmd++) 
    {
        printf("  Cmd[%d]: ", cmd);
        command_table[cmd](100);
    }
    printf("\n");

    // 함수 포인터 캐스팅
    printf("10. Function Pointer Casting:\n");
    call_generic((GenericFunc)specific_func);
    printf("\n");

    // 재귀 함수 포인터
    printf("11. Recursive Function Pointer:\n");
    printf("  5! = %d\n\n", factorial_impl(5, factorial_impl));

    // 전략 패턴
    printf("12. Strategy Pattern:\n");
    int dataset[] = { 2, 3, 4 };
    for (int i = 0; i < 3; i++) 
    {
        printf("  %s: %d\n", strategies[i].name,
            strategies[i].execute(dataset, 3));
    }

    return 0;
}