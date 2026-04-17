#pragma once

// 구조체 선언
struct MyVector
{
	int* data;			// 현재 해당 vector가 소유한 int 포인터.
	size_t size;		// 현재 vector의 크기.
	size_t capacity;	// 해당 vector의 최대 용량.
};
typedef struct MyVector MyVector;

// 초기화 
void MyVectorInit(MyVector* vector);

// 할당 해제
void MyVectorFree(MyVector* vector);

// 끝에 x 추가, 성공 시 0, 실패 시 -1
int MyVectorPushBack(MyVector* vector, int value);

// 원하는 위치에 요소 추가. 실패 시 -1 반환, 성공 시 0반환.
int MyVectorInsert(MyVector* vector, const size_t position, const int* value);

// 원하는 부분을 삭제. 실패 시 -1 반환, 성공 시 0반환.
int MyVectorErase(MyVector* vector, const int first, const int last);

// capacity만 원하는 값으로 증가. 실패 시 -1 반환, 성공 시 0반환.
int MyVectorReserve(MyVector* vector, const int new_capacity);

// vector 내부를 초기화 해주는 함수이다.
int MyVectorClear(MyVector* vector);

// vector 배열의 크기를 재조정하는 함수
int MyVectorResize(MyVector* vector, const int new_size);

// capacity == size 가 되도록 메모리를 줄인다
int MyVectorShrink(MyVector* vector);

// 범위 체크, 실패 시 -1
int GetMyVector(const MyVector* vector, size_t index, int* out);