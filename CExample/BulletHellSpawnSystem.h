#pragma once

// 구조체 선언
typedef struct
{
	Vector2 position;
	Vector2 velocity;
	int ttl;		// 남은 수명 프레임
	unsigned id;	// 유니크 ID (디버깅용)
} Bullet;

// 구조체 선언
typedef struct 
{
	Bullet* data;		// 연속 메모리 (가변 배열)
	size_t size;		// 현재 활성 탄환 수
	size_t capacity;	// 할당된 용량
	unsigned next_id;	// 다음 생성될 id
} BulletVector;

// BulletVector 생성
void BulletVectorInit(BulletVector* vector);

// BulletVector 파괴
void BulletVectorDestroy(BulletVector* vector);

// BulletVector 스폰
Bullet* BulletVectorSpawn(BulletVector* vector, Vector2 position, Vector2 velocity, int ttl);

// 모든 탄환을 업데이트하고 (x += vxdt, y += vydt, --ttl)
// ttl <= 0 또는 AABB 밖이면 제거한다.
int BulletVectorUpdateAndCull(BulletVector* vector, float dt, Vector2 min, Vector2 max);

// 용량 확보 (필요 시 realloc)
int BulletVectorReserve(BulletVector* vector, size_t new_capacity);

int BulletVectorRemove(BulletVector* vector, const int index);

void BulletSystemTest();

