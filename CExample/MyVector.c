#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyVector.h"


// 초기화 
void MyVectorInit(MyVector* vector)
{
	vector->data = NULL;	// vector가 소유한 int 포인터를 NULL로 비우기.
	vector->size = 0;		// vector의 현재 크기를 0으로 설정.
	vector->capacity = 0;	// vector의 최대 용량을 0으로 설정.
}

// 할당 해제
void MyVectorFree(MyVector* vector)
{
	free(vector->data);		// 메모리 공간에 할당한 포인터를 free.
	vector->data = NULL;	// vector가 소유한 int 포인터를 NULL로 비우기.
	vector->size = 0;		// vector의 현재 크기를 0으로 설정.
	vector->capacity = 0;	// vector의 최대 용량을 0으로 설정.
}

// 끝에 x 추가, 성공 시 0, 실패 시 -1
int MyVectorPushBack(MyVector* vector, int value)
{
	// 현재 크기가 최대 용량과 같은 크기가 되었을때.
	if (vector->size == vector->capacity)
	{
		// 최대 용량을 2배로 만든다.
		size_t new_capacity = (vector->capacity == 0) ? 4 : vector->capacity * 2;

		MyVectorReserve(vector, new_capacity);
	}

	// 이제 value값을 크기에 맞는 공간에 할당.
	vector->data[vector->size++] = value;

	// 성공하였으니 0을 반환.
	return 0;
}

// 원하는 위치에 요소 추가. 실패 시 -1 반환, 성공 시 0반환.
int MyVectorInsert(MyVector* vector, const size_t position, const int* value)
{
	// position이 size보다 크면.
	if (position >= vector->size)
	{
		// 실패 시 에러 메세지 출력
		fprintf(stderr, "position이 size보다 크다\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// position이 음수라면.
	if (position < 0)
	{
		// 실패 시 에러 메세지 출력
		fprintf(stderr, "position이 음수다\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// 현재 크기가 최대 용량과 같은 크기가 되었을때.
	if (vector->size == vector->capacity)
	{
		// 최대 용량을 2배로 만든다.
		size_t new_capacity = (vector->capacity == 0) ? 4 : vector->capacity * 2;

		// Reserve 사용
		MyVectorReserve(vector, new_capacity);
	}

	// 해당 포지션에서 하나씩 밀어내기.
	memmove
	(
		&vector->data[position + 1],
		&vector->data[position],
		(vector->size - position) * sizeof(int)
	);

	// 값 삽입
	vector->data[position] = *value;
	vector->size++;

	// 성공했으니 0을 반환.
	return 0;
}

// 원하는 부분을 삭제. 실패 시 -1 반환, 성공 시 0반환.
int MyVectorErase(MyVector* vector, const int first, const int last)
{
	// vector가 NULL이라면
	if (vector == NULL)
	{
		// 실패 시 에러 메세지 출력
		fprintf(stderr, "vector가 NULL임\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// first가 음수일 경우
	if (first < 0)
	{
		// 실패 시 에러 메세지 출력
		fprintf(stderr, "first가 음수다\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// last가 현재 크기보다 크다면 
	if (last > vector->size)
	{
		// 실패 시 에러 메세지 출력
		fprintf(stderr, "last가 현재 크기보다 크다\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// first가 last보다 크다면
	if (first >= last)
	{
		// 실패 시 에러 메세지 출력
		fprintf(stderr, "first가 last보다 크다\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// 삭제할 개수
	int count = last - first;

	// 뒤쪽 요소들을 앞으로 당기기
	memmove
	(
		&vector->data[first],
		&vector->data[last],
		(vector->size - last) * sizeof(int)
	);

	// 현재 크기 동기화
	vector->size -= count;

	// 성공했으니 0을 반환.
	return 0;
}

// capacity만 원하는 값으로 증가. 실패 시 -1 반환, 성공 시 0반환.
int MyVectorReserve(MyVector* vector, const int new_capacity)
{
	// vector가 NULL이라면
	if (vector == NULL)
	{
		// 실패 시 에러 메세지 출력
		fprintf(stderr, "vector가 NULL임\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// 이미 크기가 충분한 경우
	if (new_capacity <= vector->capacity)
	{
		// 이미 충분한 경우
		return 0;
	}

	// 기존 데이터를 유지하기 위해 임시 저장 공간을 할당한다.
	void* temp = realloc(vector->data, new_capacity * sizeof(int));

	// realloc 실패 시, 기존 데이터를 유지한채로, 함수는 -1을 반환.
	if (temp == NULL)
	{
		// 실패: vector->data는 여전히 유효.
		fprintf(stderr, "realloc 실패\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// 새로운 data로 교체
	vector->data = temp;

	// 새로운 최대 용량으로 교체
	vector->capacity = new_capacity;

	// 성공했으니 0을 반환.
	return 0;
}

// vector 내부를 초기화 해주는 함수이다.
int MyVectorClear(MyVector* vector)
{
	// vector가 NULL이라면
	if (vector == NULL)
	{
		// 실패 시 에러 메세지 출력
		fprintf(stderr, "vector가 NULL임\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// size를 0으로 만듬
	vector->size = 0;

	// 다만 capactiy는 그대로 유지시켜야 하기 때문에 넘어감
	// 성공 했으니 0을 반환.
	return 0;
}

// vector 배열의 크기를 재조정하는 함수
int MyVectorResize(MyVector* vector, const int new_size)
{
	// vector가 NULL이라면
	if (vector == NULL)
	{
		// 실패 시 에러 메세지 출력
		fprintf(stderr, "vector가 NULL임\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// 만약 new_size가 음수라면
	if (new_size < 0)
	{
		// 실패 시 에러 메세지 출력
		fprintf(stderr, "new_size가 음수임\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// new_size > capacity인 경우 자동으로 capacity 확장
	if (new_size > vector->capacity)
	{
		// 최소한 new_size까지 확보
		size_t new_capacity = vector->capacity == 0 ? new_size : vector->capacity;
		while (new_capacity < (size_t)new_size)
		{
			new_capacity *= 2; // 2배씩 확장
		}

		// data, capacity 적용
		MyVectorReserve(vector, new_capacity);
	}


	// new_size < size 뒤쪽 요소 잘라낸다
	if (new_size < vector->size)
	{
		vector->size = new_size;
	}
	// new size == size 아무것도 안함
	else if (new_size == vector->size)
	{
		// 그대로 둔다.	
	}
	// new_size > size 부족한 만큼 fill_value로 채움
	else if (new_size > vector->size)
	{
		for (size_t i = vector->size; i < (size_t)new_size; i++)
		{
			vector->data[i] = 0; // 기본값 0
		}
		vector->size = new_size;
	}

	// 기존 데이터는 유지
	// 성공 했으니 0을 반환.
	return 0;
}

// capacity == size 가 되도록 메모리를 줄인다
int MyVectorShrink(MyVector* vector)
{
	// vector가 NULL이라면
	if (vector == NULL)
	{
		// 실패 시 에러 메세지 출력
		fprintf(stderr, "vector가 NULL임\n");

		// 실패 하였으므로 -1을 반환.
		return -1;
	}

	// size == 0 인 경우
	if (vector->size == 0)
	{
		// data == NULL, capacity == 0 상태로 만든다
		free(vector->data);

		// 일단 temp를 박고
		int* temp = NULL;

		// data를 NULL로
		vector->data = temp;

		// capacity를 0으로
		vector->capacity = 0;
	}

	// capacity > size 인 경우
	if (vector->capacity > vector->size)
	{
		// capacity == size 가 되도록 메모리를 줄인다
		size_t new_capacity = vector->size;

		// data, capacity 적용
		MyVectorReserve(vector, new_capacity);
	}

	// 성공 했으니 0을 반환.
	return 0;
}

// 범위 체크, 실패 시 -1
int GetMyVector(const MyVector* vector, size_t index, int* out)
{
	// 인덱스가 현재 size 범위 안에 있는지 확인
	if (index >= vector->size)
	{
		// 실패 -1을 반환.
		return -1;
	}

	// 정상 범위라면 out에 값 저장
	*out = vector->data[index];

	// 성공하였으니 0을 반환.
	return 0;
}