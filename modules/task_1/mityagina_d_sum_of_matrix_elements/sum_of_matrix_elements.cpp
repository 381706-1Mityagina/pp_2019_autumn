// Copyright 2019 Mityagina Daria
#include "./modules/task_1/mityagina_d_sum_of_matrix_elements/sum_of_matrix_elements.h"
#include <mpi.h>
#include <math.h>
#include <ctime>
#include <vector>
#include <stdexcept>


#define MIN(a, b) (a > b)? b : a;
#define MAX(a, b) (a < b)? b : a;

int Work(int size, std::vector<int> matrix) {
  int sum_res = 0, part_sum = 0;
  int rank, p_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p_size);
  int calculate_part = size / p_size;
  int dop = size % p_size;
  int part_size = calculate_part;
  std::vector<int> recieved = std::vector<int>(part_size, 0);
  if (rank == 0) {
    for (int proc = 1; proc < p_size ; proc++) {
      if (part_size > 0)
        MPI_Send(&matrix[0] + proc * calculate_part + dop, part_size, MPI_INT, proc, 1, MPI_COMM_WORLD);
    }
    std::vector<int> recieved = std::vector<int>(matrix.begin(), matrix.begin() + calculate_part + dop);
    part_sum = SumOfMatrixElementsPartly(recieved, calculate_part + dop);
  } else {
    for (int proc = 1; proc < p_size; proc++) {
      if (part_size > 0) {
        MPI_Status status;
        MPI_Recv(&recieved[0], part_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        part_sum = SumOfMatrixElementsPartly(recieved, part_size);
      }
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&part_sum, &sum_res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return sum_res;
}

int SumOfMatrixElementsPartly(std::vector<int> matrix, int size) {
  int sum = 0;
  for (int i = 0; i < size; i++)
    sum += matrix[i];
  return sum;
}

std::vector<int> getMatrix(int size) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> new_matrix(size);
  for (int i = 0; i < size; ++i) {
    new_matrix[i] = gen() % 100;
  }
  return new_matrix;
}
