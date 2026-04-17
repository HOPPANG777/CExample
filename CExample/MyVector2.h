#pragma once

// 구조체 선언
typedef struct
{
	float x;
	float y;
} Vector2;

Vector2 vector_assign(Vector2 *v1, Vector2 *v2);

Vector2 vector_add(Vector2 v1, Vector2 v2);

Vector2 vector_sub(Vector2 v1, Vector2 v2);

Vector2 vector_mul(Vector2 v1, Vector2 v2);

Vector2 vector_divide(Vector2 v1, Vector2 v2);

Vector2 vector_add_assign(Vector2 *v1, Vector2 *v2);

Vector2 vector_sub_assign(Vector2 *v1, Vector2 *v2);

Vector2 vector_mul_assign(Vector2 *v1, const float scalar);

Vector2 vector_divide_assign(Vector2 *v1, const float scalar);
