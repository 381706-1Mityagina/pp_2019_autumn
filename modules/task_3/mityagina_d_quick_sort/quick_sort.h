// Copyright 2019 Mityagina Daria
#ifndef MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_H_
#define MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_H_
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

// 0 Пусть, исходный набор данных расположен на первом (0) процессоре, с него начинается работа алгоритма:
// - Процессор выбирает ведущий элемент, сортирует остальные элементы относительно него (большие - в правую сторону, меньшие - в левую);
// - Меньшую часть он отдает другому свободному процессору;
// - После этого процессор продолжает работать с большей частью одновременно, в то время как другой процессор работает с меньшей по 
// принципу начиная с 1го пункта;
// - Когда все процессоры получат свою часть, ускорение алгоритма будет максимальным. 

std::vector<int> getRandomVector(int vectorSize);

void division(std::vector<int> _vector, int left, int right, int &t);
std::vector<int> q_sort_parallel(std::vector<int> _vector);
void merge();

int partition (std::vector<int> _vector, int p, int r);
std::vector<int> q_sort_not_parallel(std::vector<int> _vector, int p, int r);

#endif  // MODULES_TASK_3_MITYAGINA_D_QUICK_SORT_H_
