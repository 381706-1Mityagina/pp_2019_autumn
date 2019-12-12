// Copyright 2019 Mityagina Daria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include "../../../modules/task_3/mityagina_d_quick_sort/quick_sort_p.h"

TEST(Quick_Sort_MPI, Empty_matrix) {
    int size = 0;
    int rank;
    std::vector<int> _vector(size, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(main_work(_vector, size));
    }
}

void testing_lab(int size) {
    int rank;
    std::vector<int> _vector(size), result_my(size), result(size), copy(size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        _vector = getRandomVector(size);
    }
    result = main_work(_vector, size);
    if (rank == 0) {
      sort(_vector.begin(), _vector.end());
      result_my = _vector;
    }
    if (rank == 0) {
        ASSERT_EQ(result, result_my);
    }
}

TEST(Quick_Sort_MPI, Test_On_Size_1) {
    int size = 1;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_5) {
    int size = 5;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_7) {
    int size = 7;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_9) {
    int size = 9;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_91) {
    int size = 91;
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

TEST(Quick_Sort_MPI, Test_On_Size_2000) {
    int size = 2000;
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
