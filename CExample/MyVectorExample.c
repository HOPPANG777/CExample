#include "MyVectorExample.h"
#include "MyVector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void MyVectorExample()
{
	MyVector vector;
	MyVectorInit(&vector);

	// 0부터 9까지 push_back
	for (int i = 0; i < 10; ++i)
	{
		if (MyVectorPushBack(&vector, i * 10) == -1)
		{
			fprintf(stderr, "PushBack 실패\n");
			MyVectorFree(&vector);
			return -1;
		}
	}

	printf("=== 초기 상태 ===\n");
	printf("size = %zu, capactiy = %zu\n", vector.size, vector.capacity);

	for (size_t i = 0; i < vector.size; ++i)
	{
		// out 생성.
		int value;

		if (GetMyVector(&vector, i, &value) == 0)
		{
			printf("value[%zu] = %d\n", i, value);
		}
	}

	// Clear 테스트
	MyVectorClear(&vector);
	printf("\n=== Clear 후 ===\n");
	printf("size = %zu, capacity = %zu\n", vector.size, vector.capacity);

	// 다시 push_back 해서 정상 동작 확인
 	MyVectorPushBack(&vector, 42);

	printf("\n=== Clear 후 push_back ===\n");
	printf("size = %zu, capacity = %zu\n", vector.size, vector.capacity);

	for (size_t i = 0; i < vector.size; i++) 
	{
		int value;

		if (GetMyVector(&vector, i, &value) == 0)
		{
			printf("value[%zu] = %d\n", i, value);
		}
	}

	// Resize 테스트 (size 늘리기)
	MyVectorResize(&vector, 5);

	printf("\n=== Resize(5) 후 ===\n");
	printf("size = %zu, capacity = %zu\n", vector.size, vector.capacity);
	for (size_t i = 0; i < vector.size; i++) 
	{
		int value;

		if (GetMyVector(&vector, i, &value) == 0)
		{
			printf("value[%zu] = %d\n", i, value);
		}
	}

	// Resize 테스트 (size 줄이기)
	MyVectorResize(&vector, 2);

	printf("\n=== Resize(2) 후 ===\n");
	printf("size = %zu, capacity = %zu\n", vector.size, vector.capacity);

	for (size_t i = 0; i < vector.size; i++) 
	{
		int value;

		if (GetMyVector(&vector, i, &value) == 0)
		{
			printf("value[%zu] = %d\n", i, value);
		}
	}

	// Shrink 테스트
	MyVectorShrink(&vector);

	printf("\n=== Shrink 후 ===\n");
	printf("size = %zu, capacity = %zu\n", vector.size, vector.capacity);

	for (size_t i = 0; i < vector.size; i++) 
	{
		int value;

		if (GetMyVector(&vector, i, &value) == 0)
		{
			printf("value[%zu] = %d\n", i, value);
		}
	}

	MyVectorFree(&vector);

	printf("\n");

	system("pause");
}