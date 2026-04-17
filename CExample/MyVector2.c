#include <stdio.h>
#include "MyVector2.h"

Vector2 vector_assign(Vector2 *v1, Vector2 *v2)
{
	v1->x = v2->x;
	v1->y = v2->y;
	return *v1;
}

Vector2 vector_add(Vector2 v1, Vector2 v2)
{
	Vector2 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	return result;
}

Vector2 vector_sub(Vector2 v1, Vector2 v2)
{
	Vector2 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	return result;
}

Vector2 vector_mul(Vector2 v1, Vector2 v2)
{
	Vector2 result;
	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	return result;
}

Vector2 vector_divide(Vector2 v1, Vector2 v2)
{
	Vector2 result;
	result.x = v1.x / v2.x;
	result.y = v1.y / v2.y;
	return result;
}

Vector2 vector_add_assign(Vector2* v1, Vector2* v2)
{
	v1->x += v2->x;
	v1->y += v2->y;
	return *v1;
}

Vector2 vector_sub_assign(Vector2* v1, Vector2* v2)
{
	v1->x -= v2->x;
	v1->y -= v2->y;
	return *v1;
}

Vector2 vector_mul_assign(Vector2* v1, const float scalar)
{
	v1->x *= scalar;
	v1->y *= scalar;
	return *v1;
}

Vector2 vector_divide_assign(Vector2* v1, const float scalar)
{
	v1->x /= scalar;
	v1->y /= scalar;
	return *v1;
}


