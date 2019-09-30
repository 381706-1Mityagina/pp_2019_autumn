// Copyright 2019 Mityagina Daria
#ifndef _SUM_OF_MATRIX_ELEMENTS_H_
#define _SUM_OF_MATRIX_ELEMENTS_H_

#include <stdlib.h>
#include <stdio.h>
#include <vector>

/*struct cool_process
{
	int rank;
	int size;
	//int matrix_part;
	int result;
};

struct cool_part
{
	int start;
	int size;
};*/

//void InitData(cool_process *process, int **matrix, int *dataSize, int *argc, char **argv[]);

int Work(int size, std::vector<int> matrix);

// сбор данных после обработки всех частей
//void GetAllOfThePartSumm(cool_process *process, cool_part *part, int result, int size, MPI_Datatype part);

// используется в конце работы, освобождение памяти
void FreeMem();

// работа с отдельной частью, вычисление суммы
int SumOfMatrixElementsPartly(std::vector<int> matrix, int size);

std::vector<int> getMatrix(const int size);
//std::vector<int64_t> getSumValinRows(const std::vector<int>& a,
//                                     const size_t rows, const size_t cols);

#endif  // _SUM_OF_MATRIX_ELEMENTS_H_