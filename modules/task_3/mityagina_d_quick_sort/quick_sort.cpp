// Copyright 2019 Mityagina Daria
#include "../../../modules/task_3/mityagina_d_quick_sort/quick_sort.h"
#include <math.h>
#include <stdio.h>
#include <cstring>
#include <ctime>
#include <random>
#include <algorithm>
#include <stdexcept>

std::vector<int> getRandomVector(int size) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> _vector(size);
  for (int i = 0; i < size; ++i) {
    _vector[i] = gen() % 100;
  }
  return _vector;
}

std::vector<int> q_sort_parallel(std::vector<int> _vector) {
  int size = _vector.size();
  int p_size = 0, rank = 0, error, q, part_size = 0, taken;
  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &p_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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

  if (p_size > 1 && _vector.size() / p_size > 0) {
    if (rank == 0) {
      q = partition (part, 0, _vector.size());
      part_size = part.size() - q;
      if (part_size > 0) {
        MPI_Send(&_vector, part_size, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
        MPI_Send(&part_size, 1, MPI_INT, rank + 1, (rank + 1) * 10, MPI_COMM_WORLD);
        MPI_Send(&taken, 1, MPI_INT, rank + 1, (rank + 1) * 100, MPI_COMM_WORLD);
      }
    } else {
      MPI_Recv(&taken, 1, MPI_INT, status.MPI_SOURCE, (status.MPI_SOURCE + 1) * 100, MPI_COMM_WORLD, &status);
      MPI_Recv(&part_size, 1, MPI_INT, status.MPI_SOURCE, (status.MPI_SOURCE + 1) * 10, MPI_COMM_WORLD, &status);
      taken += part_size;
      part.resize(part_size);
      if (part_size > 0)
        MPI_Recv(&part, part_size, MPI_INT, status.MPI_SOURCE, (status.MPI_SOURCE + 1), MPI_COMM_WORLD, &status);

      if (rank < p_size - 1) {
        q = partition (part, 0, part.size()); 
        part_size = part.size() - q;
        if (part_size > 0) {
          MPI_Send(&part, part_size, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
          MPI_Send(&part_size, 1, MPI_INT, rank + 1, (rank + 1) * 10, MPI_COMM_WORLD);
          MPI_Send(&taken, 1, MPI_INT, rank + 1, (rank + 1) * 100, MPI_COMM_WORLD);
        }
      }
    }
  }

  if (p_size == 1) {
    _vector = part;
  }

  if (p_size > 1) {
    part = q_sort_not_parallel(part, 0, part.size() - 1);
    // слияние :
    if (rank == 0) {
      if (part_size >= 0)
        MPI_Recv(&_vector + taken, part_size, MPI_INT, MPI_ANY_SOURCE, 13, MPI_COMM_WORLD, &status);
    } else {
      if (part_size >= 0)
        MPI_Send(&part[0], part_size, MPI_INT, 0, 13, MPI_COMM_WORLD);
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
  if (p < r) {
    q = partition (_vector_res, p, r);
    q_sort_not_parallel (_vector_res, p, q - 1);
    q_sort_not_parallel (_vector_res, q + 1, r);
  }
  return _vector_res;
}