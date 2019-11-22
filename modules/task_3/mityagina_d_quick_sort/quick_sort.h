// Copyright 2019 Mityagina Daria
#ifndef MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_H_
#define MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_H_
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

std::vector<int> getRandomVector(int vectorSize);
std::vector<int> q_sort_parallel(std::vector<int> _vector);
std::vector<int> q_sort_not_parallel(std::vector<int> _vector, int p, int r);
int partition (std::vector<int> _vector, int p, int r);

#endif  // MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_H_
