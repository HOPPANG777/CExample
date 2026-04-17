#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "MyVector2.h"
#include "BulletHellSpawnSystem.h"


void BulletVectorInit(BulletVector* vector)
{
	vector->data = NULL;	// vector가 소유한 int 포인터를 NULL로 비우기.
	vector->size = 0;		// vector의 현재 크기를 0으로 설정.
	vector->capacity = 0;	// vector의 최대 용량을 0으로 설정.
	vector->next_id = 0;	// 디버깅용 id를 0으로 설정
}

void BulletVectorDestroy(BulletVector* vector)
{
	free(vector->data);		// 메모리 공간에 할당한 포인터를 free.
	vector->data = NULL;	// vector가 소유한 int 포인터를 NULL로 비우기.
	vector->size = 0;		// vector의 현재 크기를 0으로 설정.
	vector->capacity = 0;	// vector의 최대 용량을 0으로 설정.
	vector->next_id = 0;	// 디버깅용 id를 0으로 설정
}

Bullet* BulletVectorSpawn(BulletVector* vector, Vector2 position, Vector2 velocity, int ttl)
{
	// vector가 NULL일 경우에
	if (vector == NULL)
	{
		// 에러 메시지 출력 후
		// NULL 리턴
		fprintf(stderr, "vector가 NULL입니다.\n");
		return NULL;
	}

	if (vector->size + 1 > vector->capacity)
	{
		size_t new_capacity = (vector->capacity == 0) ? 4 : vector->capacity * 2;
		if (BulletVectorReserve(vector, new_capacity) != 0)
		{
			return NULL;
		}
	}

	Bullet* bullet = &vector->data[vector->size];
	{
		vector_assign(&bullet->position, &position);
		vector_assign(&bullet->velocity, &velocity);
		bullet->ttl = ttl;
		bullet->id = vector->next_id++;
	}
	
	vector->size++;

	return bullet;
}

int BulletVectorUpdateAndCull(BulletVector* vector, float dt, Vector2 min, Vector2 max)
{
	// vector가 NULL일 경우에
	if (vector == NULL)
	{
		// 에러 메시지 출력 후
		// NULL 리턴
		fprintf(stderr, "vector가 NULL입니다.\n");
		return 1;
	}

	if (vector->data == NULL)
	{
		fprintf(stderr, "vector->data가 NULL입니다.\n");
		return 1;
	}

	for (int i = 0; i < vector->size; i++)
	{
		Bullet* bullet = &vector->data[i];

		bullet->position.x += bullet->velocity.x * dt;
		bullet->position.y += bullet->velocity.y * dt;

		bullet->ttl--;

		bool outside = 
			(
				bullet->position.x < min.x ||
				bullet->position.x > max.x ||
				bullet->position.y < min.y ||
				bullet->position.y > max.y
			);

		if (bullet->ttl <= 0 || outside)
		{
			BulletVectorRemove(vector, i);
			// 삭제했으니 i는 증가하지 않고 같은 인덱스에서 다시 검사
		}
		else
		{
			i++;
		}
	}
	
	return 0;
}

int BulletVectorReserve(BulletVector* vector, size_t new_capacity)
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
	void* temp = realloc(vector->data, new_capacity * sizeof(Bullet));

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

int BulletVectorRemove(BulletVector* vector, const int index)
{	
	if (vector == NULL)
	{
		// 실패 시 1 반환
		fprintf(stderr, "vector가 NULL입니다.\n");
		return 1;
	}

	if (index < 0 || index >= vector->size)
	{
		// 실패 시 1 반환
		fprintf(stderr, "잘못된 인덱스입니다: %d\n", index);
		return 1;
	}

	// i번째 bullet 제거: 뒤에 있는 bullet들을 앞으로 shift
	for (int i = index; i < vector->size - 1; ++i)
	{
		vector->data[i] = vector->data[i + 1];
	}

	// size 감소
	vector->size--;
	
	// 성공했으니 0을 반환.
	return 0;
}

void BulletSystemTest()
{
	BulletVector v;
	Bullet* tracked[10] = { NULL };
	Vector2 min = { -100, -100 };
	Vector2 max = { 100, 100 };
	Vector2 position = { 0.0f, 0.0f };

	BulletVectorInit(&v);

	// 60fps 가정
	for (int frame = 0; frame < 60 * 20; ++frame)
	{ // 20초
		// 보스 패턴: 1프레임에 50발씩 5프레임마다
		if (frame % 5 == 0)
		{
			for (int i = 0; i < 50; ++i)
			{
				Vector2 velocity = { (float)i * 0.1f, 3.0f };
				Bullet* b = BulletVectorSpawn(&v, position, velocity, 120);
				printf("[Spawn] frame=%d, i=%d, pos=(%.2f, %.2f), vel=(%.2f, %.2f), ttl=%d\n",
					frame, i, position.x, position.y, velocity.x, velocity.y, b->ttl);
				// "추적용 핸들"처럼 포인터를 저장해둔다 (의도적으로 위험)
				if (frame == 0 && i < 10)
				{
					tracked[i] = b;
					printf("[Track] tracked[%d] = Bullet@%p\n", i, (void*)b);
				}
			}
		}

		BulletVectorUpdateAndCull(&v, 1.0f / 60.0f, min, max);
		printf("[Update] frame=%d, bullet count=%zu\n", frame, v.size);

		// tracked 포인터를 이용해 특정 탄환을 건드리는 게임 로직 (위험)
		if (frame == 200)
		{
			for (int i = 0; i < 10; ++i)
			{

				if (tracked[i])
				{
					printf("[Extend TTL] tracked[%d] before ttl=%d\n", i, tracked[i]->ttl);

					tracked[i]->ttl += 60; // 1초 연장

					printf("[Extend TTL] tracked[%d] after ttl=%d\n", i, tracked[i]->ttl);
				}
				else
				{
					printf("[Extend TTL] tracked[%d] is null\n", i);
				}
			}
		}
	}

	BulletVectorDestroy(&v);
}

