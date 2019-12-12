// Copyright 2019 Mityagina Daria
#ifndef MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_QUICK_SORT_P_H_
#define MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_QUICK_SORT_P_H_
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

// Процессор выбирает ведущий элемент, сортирует остальные элементы относительно него
// (большие - в правую сторону, меньшие - в левую);
// Меньшую часть он отдает другому свободному процессору;
// После этого процессор продолжает работать с большей частью одновременно, в то время
// как другой процессор работает с меньшей по принципу начиная с 1го пункта;

std::vector<int> getRandomVector(int vectorSize);
void quick_s(const std::vector<int> &_vector, int left, int right);
std::vector<int> Merge_my_vectors(const std::vector<int> &my_vector1, const std::vector<int> &my_vector2, int m, int n);
std::vector<int> main_work(std::vector<int> my_vector, int N);
void part(const std::vector<int> &_vector, int left, int right, const int &t);

#endif  // MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_QUICK_SORT_P_H_
