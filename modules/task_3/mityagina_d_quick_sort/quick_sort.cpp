// Copyright 2019 Mityagina Daria
#include "../../../modules/task_3/mityagina_d_quick_sort/quick_sort.h"
#include <math.h>
#include <stdio.h>
#include <cstring>
#include <ctime>
#include <random>
#include <algorithm>
#include <stdexcept>

std::vector<int> getRandomVector(int vectorSize) {
    // std::mt19937 gen;
    // gen.seed((unsigned)time(0) + ++offset);
    std::vector<int> vec(vectorSize);
    for (int i = 0; i < vectorSize; ++i)
        vec[i] = rand() % 100;
    return vec;
}

// void division(std::vector<int> _vector, int left, int right, int &t){
//  int x = _vector[left];
//  int tmp = 0;
//  t = left;
//  for(int i = left + 1; i <= right; i++) {
//   if(_vector[i] < x) {
//    t++;
//    tmp = _vector[t];
//    _vector[t] = _vector[i];
//    _vector[i] = tmp;
//   }
//  }
//  tmp = _vector[left];
//  _vector[left] = _vector[t];
//  _vector[t] = tmp;
// }

std::vector<int> q_sort_parallel(std::vector<int> _vector) {
  int size = _vector.size();
  int p_size = 0, rank = 0, error;
  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &p_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int calculate_part = size / p_size;
  const int dop = size % p_size;

  if (rank == 0) {
    if (size <= 0) {
      error = 1;
    } else {
      error = 0;
    }
    for (int proc = 1; proc < p_size; ++proc)
      MPI_Send(&error, 1, MPI_INT, proc, 5, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&error, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
  }
  switch (error) {
  case 0:
    break;
  case 1:
    throw std::runtime_error("size <= 0");
  }

  std::vector<int> part = _vector;

  // int plength = 0;
  // int lproc = size - 1;
  // int fproc = rank;
  
  // if(rank != 0) {
  //   MPI_Recv(&plength, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
  //   part.resize(plength);
  //   if (plength > 0)
  //     MPI_Recv(&part[0], plength, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
  //   MPI_Recv(&lproc, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
  // }
  // //else {
  //   int m;
  //   int l = 0;
  //   int r = plength - 1;
  //   int x = fproc + (lproc - fproc + 1) / 2;
  //   int y = lproc;
  //   int pos;
  //   int count;

  //   while (x > fproc) {
  //     division(part, l, r, m);
  //     pos = 0;
  //     count = 0;
  //     if (m - l + 1 <= r - m) {
  //       pos = l;
  //       count = m - l + 1;
  //       l = m + 1;
  //     } else {
  //       pos = m + 1;
  //       count = r - m;
  //       r = m;
  //     }
  //     if (count > 0) {
  //       MPI_Send(&count, 1, MPI_INT, x, 0, MPI_COMM_WORLD);
  //       MPI_Send(&_vector[0] + pos, count, MPI_INT, x, 0, MPI_COMM_WORLD);
  //     }
  //     MPI_Send(&y, 1, MPI_INT, x, 0, MPI_COMM_WORLD);

  //     y = x - 1;
  //     x = fproc + (y - fproc + 1) / 2;
  //   }
    // if (rank != 0) {
    //   MPI_Send(&part[0], count, MPI_INT, 0, 10, MPI_COMM_WORLD);
    // } else {
    //   if (size > 1)
    //     MPI_Recv(&_vector[0] + pos, plength, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, &status);
    // }

  std::vector<int> recieved = std::vector<int>(calculate_part, 0);

  if (rank == 0) {
    if (calculate_part != 0) {
      for (int proc = 1; proc < p_size; proc++)
        MPI_Send(&_vector[dop] + proc * calculate_part, calculate_part, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }
  }
  if (rank == 0) {
    recieved.resize(calculate_part + dop);
    recieved = std::vector<int>(_vector.begin(), _vector.begin() + calculate_part + dop);
  } else if (calculate_part != 0) {
      MPI_Recv(&recieved[0], calculate_part, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }
  recieved = q_sort_not_parallel(recieved, 0, recieved.size() - 1);
  if (p_size > 1) {
    if (rank == 0)
    {
      MPI_Recv(&_vector[0] + calculate_part * (rank + 1) + dop, calculate_part, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, &status);
    } else {
      MPI_Send(&recieved[0], calculate_part, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }
  }

  return _vector;
}

int partition (std::vector<int> _vector, int p, int r) {
    double x = _vector[r];
    int i = p - 1;
    int j;
    double tmp;
    for (j = p; j < r; j++)
    {
      if (_vector[j] <= x)
      {
        i++;
        tmp = _vector[i];
        _vector[i] = _vector[j];
        _vector[j] = tmp;
      }
    }
    tmp = _vector[r];
    _vector[r] = _vector[i + 1];
    _vector[i + 1] = tmp;
    return i + 1;
}

std::vector<int> q_sort_not_parallel(std::vector<int> _vector, int p, int r) {
  int q;
  std::vector<int> _vector_res(_vector);  
  if (p < r)    {
    q = partition (_vector_res, p, r);
    q_sort_not_parallel (_vector_res, p, q - 1);
    q_sort_not_parallel (_vector_res, q + 1, r);
  }
  return _vector_res;
}
