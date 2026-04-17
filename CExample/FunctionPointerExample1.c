#include <stdio.h>

// 기본 연산 함수들
int Add(int a, int b) { return a + b; }

int Sub(int a, int b) { return a - b; }

int Mul(int a, int b) { return a * b; }

int Div(int a, int b) { return b ? a / b : 0; }

// 배열의 각 요소에 연산 적용
void Map(int* arr, int size, int (*op)(int)) 
{
    for (int i = 0; i < size; i++) 
    {
        arr[i] = op(arr[i]);
    }
}

int Square(int x) { return x * x; }

int Twice(int x) { return x * 2; }

// 계산기 - 연산자로 함수 선택
int Calculate(int a, int b, char op) 
{
    // 함수 포인터 타입 정의
    typedef int (*Operation)(int, int);

    // 함수 포인터 배열로 연산 매핑
    Operation ops[] = { Add, Sub, Mul, Div };
    const char* symbols = "+-*/";

    for (int i = 0; symbols[i]; i++) 
    {
        if (op == symbols[i]) 
        {
            return ops[i](a, b);  // 선택된 함수 실행
        }
    }
    return 0;
}

// 정렬 비교 함수들
int Ascending(int a, int b) { return a - b; }

int Descending(int a, int b) { return b - a; }

void BubbleSort(int* arr, int size, int (*compare)(int, int)) 
{
    for (int i = 0; i < size - 1; i++) 
    {
        for (int j = 0; j < size - i - 1; j++) 
        {
            if (compare(arr[j], arr[j + 1]) > 0) 
            {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void FunctionPointerExampleTest() 
{
    // 직접 호출 vs 포인터 호출
    int (*func_ptr)(int, int) = Add;
    printf("Direct: %d, Pointer: %d\n", Add(3, 4), func_ptr(3, 4));

    // 런타임에 함수 선택
    func_ptr = Mul;
    printf("Switched: %d\n", func_ptr(3, 4));

    // 함수 포인터로 계산기
    printf("10 + 5 = %d\n", Calculate(10, 5, '+'));
    printf("10 * 5 = %d\n", Calculate(10, 5, '*'));

    // 배열 변환
    int numbers[] = { 1, 2, 3, 4, 5 };
    Map(numbers, 5, Square);
    printf("Squared: ");
    for (int i = 0; i < 5; i++) printf("%d ", numbers[i]);
    printf("\n");

    // 정렬 방향 제어
    int data[] = { 5, 2, 8, 1, 9 };
    BubbleSort(data, 5, Ascending);
    printf("Ascending: ");
    for (int i = 0; i < 5; i++) printf("%d ", data[i]);
    printf("\n");

    BubbleSort(data, 5, Descending);
    printf("Descending: ");
    for (int i = 0; i < 5; i++) printf("%d ", data[i]);
    printf("\n");
}