// Copyright 2019 Mityagina Daria
#ifndef MODULES_TASK_1_MITYAGINA_D_SUM_OF_MATRIX_ELEMENTS_SUM_OF_MATRIX_ELEMENTS_H_
#define MODULES_TASK_1_MITYAGINA_D_SUM_OF_MATRIX_ELEMENTS_SUM_OF_MATRIX_ELEMENTS_H_

#include <stdlib.h>
#include <stdio.h>
#include <vector>

int Work(int size, std::vector<int> matrix);

// используется в конце работы, освобождение памяти
void FreeMem();

// работа с отдельной частью, вычисление суммы
int SumOfMatrixElementsPartly(std::vector<int> matrix, int size, int offset);

std::vector<int> getMatrix(const int size);

#endif  // MODULES_TASK_1_MITYAGINA_D_SUM_OF_MATRIX_ELEMENTS_SUM_OF_MATRIX_ELEMENTS_H_
