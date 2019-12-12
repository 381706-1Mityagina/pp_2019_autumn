// Copyright 2019 Mityagina Daria
#include "../../../modules/task_3/mityagina_d_quick_sort/quick_sort_p.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cstring>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>

std::vector<int> getRandomVector(int size) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> _vector(size);
  for (int i = 0; i < size; ++i) {
    _vector[i] = gen() % 100;
  }
  return _vector;
}

// int part(const std::vector<int> &_vector, int left, int right) {
//   int x = _vector[left];
//   int tmp = 0;
//   t = left;
//   for (int i = left + 1; i <= right; i++) {
//     if (_vector[i] < x) {
//       t++;
//       tmp = _vector[t];
//      *_vector[t] = _vector[i];
//      _vector[i] = tmp;
//     }
//   }
//   tmp = _vector[left];
//   _vector[left] = _vector[t];
//   _vector[t] = tmp;

//   return t;
// }

std::vector<int> quick_s(std::vector<int> _vector, int left, int right) {
  if (left < right) {
    // int t = part(_vector, left, right);
    int x = _vector[left];
    int tmp = 0;
    int t = left;
    for (int i = left + 1; i <= right; i++) {
      if (_vector[i] < x) {
        t++;
        tmp = _vector[t];
       _vector[t] = _vector[i];
       _vector[i] = tmp;
      }
    }
    tmp = _vector[left];
    _vector[left] = _vector[t];
    _vector[t] = tmp;

    quick_s(_vector, left, t);
    quick_s(_vector, t + 1, right);
  }
  return _vector;
}

std::vector<int> Merge_my_vectors(const std::vector<int> &mv1, const std::vector<int> &mv2, int m, int n) {
  int i = 0, j = 0, k = 0;
  std::vector<int> result = std::vector<int>(m + n);
  while (i < m && j < n) {
    if (mv1[i] <= mv2[j]) {
      result[k] = mv1[i];
      i++;
    } else {
      result[k] = mv2[j];
      j++;
    }
    k++;
  }
  if (i < m) {
    for (int p = i; p < m; p++) {
      result[k] = mv1[p];
      k++;
    }
  } else {
    for (int p = j; p < n; p++) {
      result[k] = mv2[p];
      k++;
    }
  }
  return result;
}

std::vector<int> main_work(std::vector<int> my_vector, int N) {
  int rank, size, error;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int eachProc = (size > 1) ? (N / size) : N;
  int additional = N % size;
  MPI_Status st;
  std::vector<int> sub_my_vector;
  std::vector<int> result = std::vector<int>(my_vector);

  if (rank == 0) {
    if (N <= 0) {
      error = 1;
    } else {
      error = 0;
    }
    for (int proc = 1; proc < size; ++proc)
        MPI_Send(&error, 1, MPI_INT, proc, 100, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&error, 1, MPI_INT, 0, 100, MPI_COMM_WORLD, &st);
    }
    switch (error) {
        case 0:
            break;
        case 1:
            throw std::runtime_error("size <= 0");
  }
  if (size > 1 && N / size > 0) {
    std::vector<std::vector<int>> new_vec = std::vector<std::vector<int>>(size - 1, std::vector<int>(eachProc, 0));
    if (rank == 0) {
      sub_my_vector = std::vector<int>(eachProc + additional, 0);
    } else if (rank > 0) {
      sub_my_vector = std::vector<int>(eachProc, 0);
    }
    if (rank == 0) {
      for (int i = 1; i < size; i++) {
        if (eachProc * i + additional <= N - eachProc)
        MPI_Send(&my_vector[additional] + eachProc * i, eachProc, MPI_INT, i, i, MPI_COMM_WORLD);
      }
    }
    if (rank == 0) {
      sub_my_vector.resize(eachProc + additional);
      sub_my_vector = std::vector<int>(my_vector.begin(), my_vector.begin() + eachProc + additional);
    } else {
      MPI_Recv(&sub_my_vector[0], eachProc, MPI_INT, 0, rank, MPI_COMM_WORLD, &st);
    }
    int right = (rank == 0)? eachProc + additional - 1 : eachProc - 1;
    sub_my_vector = quick_s(sub_my_vector, 0, right);
    for (int i = 0; i < eachProc + additional; i++) {
      result[i] = sub_my_vector[i];
    }
    if (rank != 0) {
      MPI_Send(&sub_my_vector[0], eachProc, MPI_INT, 0, rank * 10, MPI_COMM_WORLD);
    } else {
      for (int i = 1; i < size; i++) {
        MPI_Recv(&result[additional] + eachProc * i, eachProc, MPI_INT, MPI_ANY_SOURCE, i * 10, MPI_COMM_WORLD, &st);
      }
    }

    if (rank != 0) {
      MPI_Send(&sub_my_vector, eachProc, MPI_INT, 0, rank, MPI_COMM_WORLD);
    } else {
      new_vec[0] = std::vector<int>(sub_my_vector.begin(), sub_my_vector.begin() + eachProc + additional);
      for (int i = 1; i < size; i++) {
        MPI_Recv(&new_vec[i - 1], eachProc, MPI_INT, i, i, MPI_COMM_WORLD, &st);
      }
    }
    sort(result.begin(), result.end());
    return result;
  } else {
    my_vector = quick_s(my_vector, 0, N - 1);
    return my_vector;
  }
}
