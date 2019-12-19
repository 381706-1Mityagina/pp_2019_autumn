// Copyright 2019 Mityagina Daria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include "../../../modules/task_3/mityagina_d_quick_sort/quick_sort_p.h"

void testing_lab(int size) {
    int rank, p_size;
    double t1, t2;
    std::vector<int> _vector(size), result_my(size), result(size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p_size);
    if (rank == 0) {
        _vector = getRandomVector(size);
    }
    t1 = MPI_Wtime();
    result = std::vector<int>(main_work(_vector, size));
    t2 = MPI_Wtime();
    if (rank == 0) {
      if (p_size > 1)
        std::cout << "Parallel " << t2 - t1 << "\n";
      t1 = MPI_Wtime();
      quick_s(_vector, 0, size - 1);
      t2 = MPI_Wtime();
      sort(_vector.begin(), _vector.end());
      if (p_size == 1)
        std::cout << "Not parallel " << t2 - t1 << "\n";
      result_my = std::vector<int>(_vector);
    }
    if (rank == 0) {
        ASSERT_EQ(result, result_my);
    }
}

TEST(Quick_Sort_MPI, Test_On_Size_10000) {
    int size = 10000;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_1) {
    int size = 1;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_5) {
    int size = 6;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_7) {
    int size = 8;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_9) {
    int size = 90;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_91) {
    int size = 900;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_200) {
    int size = 200;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_1000) {
    int size = 1000;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Negative_matrix_size) {
    int size = -10;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(std::vector<int> _vector(size, 0));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
