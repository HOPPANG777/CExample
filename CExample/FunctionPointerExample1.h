#pragma once

int Add(int a, int b);

int Sub(int a, int b);

int Mul(int a, int b);

int Div(int a, int b);

void Map(int* arr, int size, int (*op)(int));

int Square(int x);

int Twice(int x);

int Calculate(int a, int b, char op);

int Ascending(int a, int b);

int Descending(int a, int b);

void BubbleSort(int* arr, int size, int (*compare)(int, int));

void FunctionPointerExampleTest();